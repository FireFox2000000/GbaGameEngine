#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/base/core/stl/Bitmask.h"

#include "engine/audio/AudioChannelProperties.h"

#include "engine/gba/audio/GBADirectSound.h"
#include "engine/gba/registers/clock/GBATimer.h"
#include "engine/gba/memory/GBADirectMemoryAccess.h"
#include "engine/time/Time.h"

namespace GBA
{
	namespace Audio
	{
		class AudioManager
		{
		public:
			// TODO, move these into some kind of internal base audio manager
			using tChannelHandle = int;
			static constexpr tChannelHandle INVALID_CHANNEL = -1;

		private:		
			void SetMasterSoundEnabled(bool enabled);

			struct SoundProperties
			{
				Array<float, AudioChannelProperties::AttributesCount> attributes;
				Bitmask<int> flags = 0;

				SoundProperties();
			};

			struct DirectSoundChannel : SoundProperties
			{
				int sampleRate = -1;
				int sampleCount = -1;
				const u8* samples = nullptr;

				// Only set if the channel is active
				GBA::Audio::DirectSound::Channels soundChannelId = GBA::Audio::DirectSound::Channels::ChannelCount;
				GBA::DirectMemoryAccess::Channels dmaChannelId = GBA::DirectMemoryAccess::Channels::Count;
				GBA::Audio::DirectSound::Timer dmaTimerId = GBA::Audio::DirectSound::Timer::SoundTimerCount;
			};

			struct RepeatParams
			{
				int sampleStartOffset;
				u32 totalSeconds;
				u32 remainder;
				u32 ticksPerSampleTransfer;
			};

			// For the majority of Update we only need to loop over expectedEofTime, so SOA more efficent 
			struct ActiveChannelSOA
			{
				static constexpr int MAX_ACTIVE_CHANNELS = 1;

				FixedList<tChannelHandle, MAX_ACTIVE_CHANNELS> handles;
				FixedList<Time::InternalSnapshot, MAX_ACTIVE_CHANNELS> expectedEofTime;
				FixedList<RepeatParams, MAX_ACTIVE_CHANNELS> repeatParams;
			};

			Array<DirectSoundChannel, 1> m_channels;		// TODO, probably want to use pool type here
			ActiveChannelSOA m_activeChannels;

			const DirectSoundChannel* GetDirectSoundChannel(tChannelHandle handle) { return &m_channels[handle]; }
			DirectSoundChannel* EditDirectSoundChannel(tChannelHandle handle) { return &m_channels[handle]; }
			const SoundProperties* GetChannelProperties(tChannelHandle handle) const;
			SoundProperties* EditChannelProperties(tChannelHandle handle);

			void PlayDirectSound(tChannelHandle handle);
			tChannelHandle CreateDirectSoundChannel(int sampleRate, int sampleCount, const u8* samples, int flags = 0);

			void PlayDirectSound(
				DirectSound::Channels soundChannel
				, DirectMemoryAccess::Channels dmaChannel
				, DirectSound::Timer dmaTimer
				, const u8* samples
				, const RepeatParams& repeatParams
				, Time::InternalSnapshot* out_endTime
			);

			void OnActiveChannelReachedEof(int activeChannelIndex);

			enum Channels
			{
				Square1,
				Square2,
				Wave,
				Noise,

				Count
			};

			// Channel will remain active but playback will be frozen for fast resume
			void Pause(const tChannelHandle& handle);		// TODO, move into public once implementation is finished
			void Resume(const tChannelHandle& handle);		// TODO, move into public once implementation is finished

		protected:
			void Init();

		public:
			// This is called multiple times per-frame in order to stop audio channels as on-time as possible. Want to avoid playing garbage. 
			void Update();

			void Stop(const tChannelHandle& handle);

			// Signed 8-bit PCM files only
			tChannelHandle CreateFromFile(const u32* file);

			// Plays an Signed 8-bit PCM file, one-shot auto disposed
			void PlayFromFile(const u32* file, float playrate = 1.0f);
			void Play(const tChannelHandle handle);

			bool GetChannelFlag(tChannelHandle handle, AudioChannelProperties::Flags flag);
			void SetChannelFlag(tChannelHandle handle, AudioChannelProperties::Flags flag, bool value);

			float GetChannelAttribute(tChannelHandle handle, AudioChannelProperties::Attributes attribute);
			void SetChannelAttribute(tChannelHandle handle, AudioChannelProperties::Attributes attribute, float value);
		};
	}
}
