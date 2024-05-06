#include "GBAAudioManager.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/io/filestream/CppFileReader.h"
#include "engine/gba/config/GBATimerId.h"
#include "GBASDK/Timers.h"

struct SoundStatusRegister
{
	u16 sqr1 : 1		// READ-ONLY
		, sqr2 : 1		// READ-ONLY
		, wave : 1		// READ-ONLY
		, noise : 1		// READ-ONLY
		, : 3
		, masterSoundEnabled : 1
		;
};

volatile SoundStatusRegister& soundStatus = (*reinterpret_cast<volatile SoundStatusRegister*>(REG_SND_STATUS));

// (2 ^ 24)hz or cycles per second
#define CLOCK 16777216

// Calculate and remove 0.03 seconds worth of ticks to account for framerate variations at 30fps, otherwise we may end up putting garbage into the speakers briefly.
constexpr int EndTimeOffset = (CLOCK * 0.03f);

GBA::Audio::AudioManager::AudioManager() 
	: m_availableDSoundChannels({ DirectSound::Channels::ChannelA, DirectSound::Channels::ChannelB })
	, m_availableDMAChannels({ DirectMemoryAccess::Sound0, DirectMemoryAccess::Sound1 })
	, m_availableDSoundTimers({ DirectSound::DSoundTimer::Sound0, DirectSound::DSoundTimer::Sound1 })
{
}

GBA::Audio::AudioManager::SoundProperties::SoundProperties()
{
	attributes[AudioChannelProperties::Attributes::Playrate] = 1.0f;
	attributes[AudioChannelProperties::Attributes::Volume] = 1.0f;
}

GBATimerId GetTimerIdForDirectSound(GBA::Audio::DirectSound::DSoundTimer dmaTimer)
{
	switch (dmaTimer)
	{
	case GBA::Audio::DirectSound::DSoundTimer::Sound0:
	{
		return GBATimerId::Sound0;
	}
	case GBA::Audio::DirectSound::DSoundTimer::Sound1:
	{
		return GBATimerId::Sound1;
	}
	default:
	{
		break;
	}
	}

	DEBUG_ERROR("Unable to get gba timer for direct sound");
	return GBATimerId::Sound0;
}

void GBA::Audio::AudioManager::Init()
{
	SetMasterSoundEnabled(true);
}

void GBA::Audio::AudioManager::SetMasterSoundEnabled(bool enabled)
{
	soundStatus.masterSoundEnabled = enabled;
}

const GBA::Audio::AudioManager::DirectSoundChannel * GBA::Audio::AudioManager::GetDirectSoundChannel(tChannelHandle handle)
{
	DEBUG_ASSERTMSG(IsDirectSoundChannel(handle), "Trying to get direct sound channel from a sound handle that is not direct sound!");

	DirectSoundChannel* channelHandle = (DirectSoundChannel*)handle;
	return channelHandle;
}

GBA::Audio::AudioManager::DirectSoundChannel * GBA::Audio::AudioManager::EditDirectSoundChannel(tChannelHandle handle)
{
	DEBUG_ASSERTMSG(IsDirectSoundChannel(handle), "Trying to get direct sound channel from a sound handle that is not direct sound!");

	DirectSoundChannel* channelHandle = (DirectSoundChannel*)handle;
	return channelHandle;
}

const GBA::Audio::AudioManager::SoundProperties * GBA::Audio::AudioManager::GetChannelProperties(tChannelHandle handle) const
{
	return (SoundProperties*)handle;
}

GBA::Audio::AudioManager::SoundProperties * GBA::Audio::AudioManager::EditChannelProperties(tChannelHandle handle)
{
	return (SoundProperties*)handle;
}

void GBA::Audio::AudioManager::PlayDirectSound(tChannelHandle handle)
{
	/*
	Initializing DMA-Sound Playback
	- Select Timer 0 or 1 in SOUNDCNT_H (0x4000082) control register.
	- Clear the FIFO.
	- Manually write a sample byte to the FIFO/place start address
	- Initialize transfer mode for DMA 1 or 2.
	- Initialize DMA Sound settings in sound control register.
	- Start the timer.

	DMA-Sound Playback Procedure
	The pseudo-procedure below is automatically repeated.
	  If Timer overflows then
		Move 8bit data from FIFO to sound circuit.
		If FIFO contains only 4 x 32bits (16 bytes) then
		  Request more data per DMA
		  Receive 4 x 32bit (16 bytes) per DMA
		Endif
	  Endif
	This playback mechanism will be repeated forever, regardless of the actual length of the sample buffer.
	*/
	using namespace GBA;

	if (handle == INVALID_CHANNEL)
	{
		DEBUG_LOG("Unable to play invalid channel");
		return;
	}

	if (m_activeChannels.handles.Count() >= m_activeChannels.handles.Capacity())
	{
		DEBUG_LOG("Unable to play channel, out of space for active channels");
		return;
	}

	if (m_availableDSoundChannels.Count() <= 0)
	{
		DEBUG_LOG("Unable to play direct sound, all channels already in use");
		return;
	}

	// Assign a channel to use. Won't be able to have a second track playing if stero sound is implemented, possibly. 
	int availableIndex = 0;
	DirectSound::Channels soundChannel = m_availableDSoundChannels[availableIndex];		m_availableDSoundChannels.RemoveAt(availableIndex);
	DirectMemoryAccess::Channels dmaChannel = m_availableDMAChannels[availableIndex];	m_availableDMAChannels.RemoveAt(availableIndex);
	DirectSound::DSoundTimer dmaTimer = m_availableDSoundTimers[availableIndex];				m_availableDSoundTimers.RemoveAt(availableIndex);

	DEBUG_LOGFORMAT("[AudioManager::PlayDirectSound] soundChannel = %d, dmaChannel = %d, dmaTimer = %d", soundChannel, dmaChannel, dmaTimer);

	// Mark any active channel in this slot as inactive, we're about to stomp the audio for this new one
	for (int i = m_activeChannels.handles.Count() - 1; i >= 0; --i)
	{
		tChannelHandle activeHandle = m_activeChannels.handles[i];
		if (IsDirectSoundChannel(activeHandle))
		{
			const auto* channel = GetDirectSoundChannel(activeHandle);
			if (channel->soundChannelId == soundChannel)
			{
				Stop(activeHandle);

				if (channel->flags.TestBit(AudioChannelProperties::DisposeOnCompletion))
				{
					FreeChannel(handle);
				}
			}
		}
	}

	DirectSoundChannel& channel = *EditDirectSoundChannel(handle);
	int requestedSampleStartOffset = 0;
	int sampleStartOffset = MIN(channel.sampleCount, requestedSampleStartOffset);

	// Assign the hardware sound channels to our sound channel handle now that it's active
	channel.soundChannelId = soundChannel;
	channel.dmaChannelId = dmaChannel;
	channel.dmaTimerId = dmaTimer;

	int sampleRate = channel.sampleRate;
	float playRate = channel.attributes[AudioChannelProperties::Playrate];

	u32 ticksPerSampleTransfer = (CLOCK / (float)sampleRate) / playRate;
	if (ticksPerSampleTransfer > u16(-1))
	{
		DEBUG_ERRORFORMAT("Ticks per sample overflow detected. Playrate (%f) may be too low or samplerate (%d) too high.", playRate, sampleRate);
		ticksPerSampleTransfer = u16(-1);
	}

	// Calculate EOF system time so that we know when to stop or repeat the sound channel, otherwise we'll end up continuing into playing garbage
	int totalSamplesToTransfer = channel.sampleCount - sampleStartOffset;
	u32 totalTicksTillEnd = totalSamplesToTransfer * ticksPerSampleTransfer - EndTimeOffset;
	u32 totalPlaybackTicks = totalTicksTillEnd / Time::ClockFreq;
	u32 totalSeconds = totalPlaybackTicks / u16(-1);
	u32 remainder = totalPlaybackTicks % u16(-1);

	float volumeAttr = channel.attributes[AudioChannelProperties::Volume];
	DirectSound::VolumeOptions vol = volumeAttr > 0.75f ? DirectSound::Vol100 : DirectSound::Vol50;		// Direct sound doesn't support precise volume control
	DirectSound::SetChannelOptions(
		soundChannel,
		DirectSound::SoundEnabledLeft | DirectSound::SoundEnabledRight | DirectSound::FifoReset,
		vol,
		dmaTimer);

	RepeatParams repeatParams = { sampleStartOffset = sampleStartOffset, totalSeconds = totalSeconds, remainder = remainder, ticksPerSampleTransfer = ticksPerSampleTransfer };
	Time::InternalSnapshot endTime;
	PlayDirectSound(soundChannel, dmaChannel, dmaTimer, channel.samples, repeatParams, &endTime);

	// Mark the channel as active for tracking
	channel.flags.SetBit(AudioChannelProperties::Active);
	m_activeChannels.handles.Add(handle);
	*m_activeChannels.expectedEofTime.AddNew() = endTime;
	*m_activeChannels.repeatParams.AddNew() = repeatParams;
}

void GBA::Audio::AudioManager::Update()
{
	auto currentTime = Time::CaptureSystemTimeSnapshot();

	for (int i = m_activeChannels.expectedEofTime.Count() - 1; i >= 0; --i)
	{
		if (currentTime >= m_activeChannels.expectedEofTime[i])
		{
			OnActiveChannelReachedEof(i);
		}
	}
}

void GBA::Audio::AudioManager::Pause(const tChannelHandle & handle)
{
	// Stop playback but leave active for easy resume.
	const auto* stream = GetDirectSoundChannel(handle);
	auto timerId = GetTimerIdForDirectSound(stream->dmaTimerId);
	auto& timer = *GBA::ioRegisterTimers[timerId];
	timer.isEnabled = false;
}

void GBA::Audio::AudioManager::Resume(const tChannelHandle & handle)
{
	// NOT SUPPORTED
	// TODO, need to recalculate EOF time

	if (m_activeChannels.handles.Contains(handle))
	{
		/*
		const auto& stream = GetDirectSoundChannel(handle);

		auto timerId = GetTimerIdForDirectSound(stream.dmaTimerId);
		auto& timer = GBA::Timers::GetTimer(timerId);
		timer.SetActive(true);
		*/
	}
	else
	{
		DEBUG_ERROR("Unable to resume audio channel that was not active to begin with.");
	}
}

void GBA::Audio::AudioManager::Stop(const tChannelHandle & handle)
{
	Pause(handle);

	for (int i = m_activeChannels.handles.Count() - 1; i >= 0; --i)
	{
		if (handle == m_activeChannels.handles[i])
		{
			auto* channel = EditChannelProperties(handle);
			channel->flags.ClearBit(AudioChannelProperties::Active);

			m_activeChannels.handles.RemoveAt(i);
			m_activeChannels.expectedEofTime.RemoveAt(i);
			m_activeChannels.repeatParams.RemoveAt(i);

			if (IsDirectSoundChannel(handle))
			{
				const auto* dSoundChannel = GetDirectSoundChannel(handle);

				DirectMemoryAccess::Reset(dSoundChannel->dmaChannelId);

				// Return direct sound channel as available again.
				m_availableDSoundChannels.Add(dSoundChannel->soundChannelId);
				m_availableDMAChannels.Add(dSoundChannel->dmaChannelId);
				m_availableDSoundTimers.Add(dSoundChannel->dmaTimerId);
			}

			return;
		}
	}
}

GBA::Audio::AudioManager::tChannelHandle GBA::Audio::AudioManager::CreateDirectSoundChannel(int sampleRate, int sampleCount, const u8* samples, int flags)
{
	DirectSoundChannel* newChannel = m_directSoundChannelPool.CreateNew();

	if (!newChannel)
	{
		DEBUG_ERROR("Unable to create a direct sound channel, none available from pool.");
		return INVALID_CHANNEL;
	}

	// We assign the hardware channels when we actually play the channel
	newChannel->soundChannelId = DirectSound::Channels::ChannelCount;
	newChannel->dmaChannelId = DirectMemoryAccess::Channels::Count;
	newChannel->dmaTimerId = DirectSound::DSoundTimer::SoundTimerCount;

	newChannel->flags = flags;
	newChannel->flags.ClearBit(AudioChannelProperties::Active);		// No read-only flags allowed

	newChannel->sampleCount = sampleCount;
	newChannel->sampleRate = sampleRate;
	newChannel->samples = samples;

	return (intptr_t)newChannel;
}

void GBA::Audio::AudioManager::PlayDirectSound(
	DirectSound::Channels soundChannel
	, DirectMemoryAccess::Channels dmaChannel
	, DirectSound::DSoundTimer dmaTimer
	, const u8* samples
	, const RepeatParams& repeatParams
	, Time::InternalSnapshot* out_endTime
) const
{
	// Direct sound timer and GBA timer must be set to the same timer, cannot mix these
	GBATimerId timerId = GetTimerIdForDirectSound(dmaTimer);

	// Reset timer and dma in case there's any previous sound playing
	auto& timer = *GBA::ioRegisterTimers[timerId];
	timer.isEnabled = false; 

	DirectMemoryAccess::Reset(dmaChannel);

	const u8* src = (samples + repeatParams.sampleStartOffset);
	vu32* dst = DirectSound::GetDestinationBuffer(soundChannel);

	// Set the dma to transfer from the sample array to the sound buffer
	// Set transfer count as 0 since transfers are repeated timer-based
	DirectMemoryAccess::Transfer(dmaChannel, dst, src, 0,
		DirectMemoryAccess::Mode::Enable |
		DirectMemoryAccess::Mode::Size32 |
		DirectMemoryAccess::Mode::Repeat |
		DirectMemoryAccess::Mode::DesinationAdjustment::Fixed |
		DirectMemoryAccess::Mode::Special);		// "Special" sets the transfer to happen based on the sound timers

	// Set the timer to overflow each time we need a new sample
	timer.SetInitialCount(65536 - repeatParams.ticksPerSampleTransfer);

	// Capture real start time just before we enable timer so it's accurate
	auto startTime = Time::CaptureSystemTimeSnapshot();

	// Start audio transfers
	timer.frequency = GBA::ClockFrequency::Cycle_1;
	timer.isEnabled = true;

	// Calculate EOF system time so that we know when to stop or repeat the sound channel, otherwise we'll end up continuing into playing garbage
	auto endTime = startTime;
	endTime.systemClockCount2 += repeatParams.totalSeconds;

	int clock1Cycles = repeatParams.remainder; // clock 1 total ticks/cycles per second
	if ((clock1Cycles + endTime.systemClockCount1) > u16(-1))
	{
		endTime.systemClockCount2 += 1;
	}
	endTime.systemClockCount1 += clock1Cycles;

	DEBUG_LOGFORMAT("Direct sound start time = (%d, %d)", startTime.systemClockCount1, startTime.systemClockCount2);
	DEBUG_LOGFORMAT("Direct sound end time = (%d, %d)", endTime.systemClockCount1, endTime.systemClockCount2);

	*out_endTime = endTime;
}

bool GBA::Audio::AudioManager::IsDirectSoundChannel(const tChannelHandle handle) const
{
	DirectSoundChannel* channelHandle = (DirectSoundChannel*)handle;
	return m_directSoundChannelPool.Contains(channelHandle);
}

void GBA::Audio::AudioManager::OnActiveChannelReachedEof(int activeChannelIndex)
{
	// Stopping the timer should be the first priorty here in order to prevent playing garbage
	tChannelHandle handle = m_activeChannels.handles[activeChannelIndex];
	Pause(handle);

	auto flags = GetChannelProperties(handle)->flags;
	if (flags.TestBit(AudioChannelProperties::Loop))
	{
		const auto& channel = *GetDirectSoundChannel(handle);
		
		PlayDirectSound(
			channel.soundChannelId
			, channel.dmaChannelId
			, channel.dmaTimerId
			, channel.samples
			, m_activeChannels.repeatParams[activeChannelIndex]
			, &m_activeChannels.expectedEofTime[activeChannelIndex]);

		DEBUG_LOGFORMAT("Channel (%d) reached expected eof, repeating playback", handle);
	}
	else
	{
		// Also need to check DisposeOnCompletion
		Stop(handle);

		if (flags.TestBit(AudioChannelProperties::DisposeOnCompletion))
		{
			FreeChannel(handle);
		}

		DEBUG_LOGFORMAT("Channel (%d) reached expected eof, stopping playback", handle);
	}
}

GBA::Audio::AudioManager::tChannelHandle GBA::Audio::AudioManager::CreateFromFile(const u32 * file)
{
	CppFileReader reader(file);
	int sampleRate = reader.Read<int>();
	int sampleCount = reader.Read<int>();
	const u8* samples = reader.ReadAddress<u8>(sampleCount);

	DEBUG_LOGFORMAT("[AudioManager::CreateFromFile] sample rate %d, sample count %d", sampleRate, sampleCount);

	return CreateDirectSoundChannel(sampleRate, sampleCount, samples);
}

void GBA::Audio::AudioManager::PlayFromFile(const u32 * file, float playrate)
{
	CppFileReader reader(file);
	int sampleRate = reader.Read<int>();
	int sampleCount = reader.Read<int>();	// Byte count is the same as sample count
	const u8* samples = reader.ReadAddress<u8>(sampleCount);

	auto handle = CreateDirectSoundChannel(sampleRate, sampleCount, samples, AudioChannelProperties::DisposeOnCompletion);

	PlayDirectSound(handle);
}

void GBA::Audio::AudioManager::Play(const tChannelHandle handle)
{
	if (IsDirectSoundChannel(handle))
	{
		PlayDirectSound(handle);
	}
	else
	{
		// Unhandled, TODO
	}
}

void GBA::Audio::AudioManager::FreeChannel(const tChannelHandle handle)
{
	if (GetChannelProperties(handle)->flags.TestBit(AudioChannelProperties::Active))
	{
		Stop(handle);
	}

	if (IsDirectSoundChannel(handle))
	{
		m_directSoundChannelPool.Free(reinterpret_cast<DirectSoundChannel*>(handle));
	}

	// ERROR?
}

bool GBA::Audio::AudioManager::GetChannelFlag(tChannelHandle handle, AudioChannelProperties::Flags flag) const
{
	const auto* properties = GetChannelProperties(handle);
	return properties->flags.TestBit(flag);
}

void GBA::Audio::AudioManager::SetChannelFlag(tChannelHandle handle, AudioChannelProperties::Flags flag, bool value)
{
	bool isReadOnlyFlag = flag == AudioChannelProperties::Active;
	if (isReadOnlyFlag)
	{
		return;
	}

	if (auto* properties = EditChannelProperties(handle))
	{
		if (value)
		{
			properties->flags.SetBit(flag);
		}
		else
		{
			properties->flags.ClearBit(flag);
		}
	}
}

float GBA::Audio::AudioManager::GetChannelAttribute(tChannelHandle handle, AudioChannelProperties::Attributes attribute) const
{
	const auto* properties = GetChannelProperties(handle);
	return properties->attributes[attribute];
}

void GBA::Audio::AudioManager::SetChannelAttribute(tChannelHandle handle, AudioChannelProperties::Attributes attribute, float value)
{
	if (auto* properties = EditChannelProperties(handle))
	{
		DEBUG_ASSERTMSGFORMAT(AudioChannelProperties::IsValidAttributeValue(attribute, value), "Invalid attribute value detected for attribute(%d), value = %f", attribute, value);

		properties->attributes[attribute] = value;
	}
}
