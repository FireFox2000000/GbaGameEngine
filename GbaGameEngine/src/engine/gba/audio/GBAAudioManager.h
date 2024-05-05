#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/base/core/stl/Bitmask.h"
#include "engine/base/core/stl/Pool.h"

#include "engine/audio/AudioChannelProperties.h"

#include "engine/gba/audio/GBADirectSound.h"
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
			using tChannelHandle = intptr_t;
			static constexpr tChannelHandle INVALID_CHANNEL = (intptr_t)nullptr;

			AudioManager();

		private:		
			static constexpr int MAX_DIRECTSOUND_CHANNELS = 2; // 1 background track + one sfx? Don't think we really need many full files sounds per scene. Most sfx should be using other channels
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
				GBA::Audio::DirectSound::DSoundTimer dmaTimerId = GBA::Audio::DirectSound::DSoundTimer::SoundTimerCount;
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

			Pool<DirectSoundChannel, MAX_DIRECTSOUND_CHANNELS> m_directSoundChannelPool;
			ActiveChannelSOA m_activeChannels;
			FixedList<DirectSound::Channels, DirectSound::Channels::ChannelCount> m_availableDSoundChannels;
			FixedList<DirectMemoryAccess::Channels, DirectMemoryAccess::Channels::SoundCount> m_availableDMAChannels;
			FixedList<DirectSound::DSoundTimer, DirectSound::DSoundTimer::SoundTimerCount> m_availableDSoundTimers;

			const DirectSoundChannel* GetDirectSoundChannel(tChannelHandle handle);
			DirectSoundChannel* EditDirectSoundChannel(tChannelHandle handle);
			const SoundProperties* GetChannelProperties(tChannelHandle handle) const;
			SoundProperties* EditChannelProperties(tChannelHandle handle);

			// Assigns GBA channels, starts playback and marks as active
			void PlayDirectSound(tChannelHandle handle);

			// Allocates sound channel from the pool
			tChannelHandle CreateDirectSoundChannel(int sampleRate, int sampleCount, const u8* samples, int flags = 0);

			// Internal usage, simply starts playback on the GBA channel. Does not mark the channel as active or change flags etc. 
			void PlayDirectSound(
				DirectSound::Channels soundChannel
				, DirectMemoryAccess::Channels dmaChannel
				, DirectSound::DSoundTimer dmaTimer
				, const u8* samples
				, const RepeatParams& repeatParams
				, Time::InternalSnapshot* out_endTime
			) const;

			bool IsDirectSoundChannel(const tChannelHandle handle) const;

			// Restarts playback if set to loop or stops and/or disposes the channel
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

			// Stops playback and marks channel inactive. Frees up device channels to be used by other sounds.
			void Stop(const tChannelHandle& handle);

			// Signed 8-bit PCM files only
			tChannelHandle CreateFromFile(const u32* file);

			// Plays an Signed 8-bit PCM file, one-shot auto disposed
			void PlayFromFile(const u32* file, float playrate = 1.0f);
			void Play(const tChannelHandle handle);

			// Stops playing the channel if active and frees the channel pool slot
			void FreeChannel(const tChannelHandle handle);

			bool GetChannelFlag(tChannelHandle handle, AudioChannelProperties::Flags flag) const;
			void SetChannelFlag(tChannelHandle handle, AudioChannelProperties::Flags flag, bool value);

			float GetChannelAttribute(tChannelHandle handle, AudioChannelProperties::Attributes attribute) const;
			void SetChannelAttribute(tChannelHandle handle, AudioChannelProperties::Attributes attribute, float value);
		};
	}
}
