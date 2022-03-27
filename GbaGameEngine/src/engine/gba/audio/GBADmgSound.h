#pragma once
#include "engine/base/Macros.h"
#include "engine/base/core/stl/List.h"
#include "engine/audio/music/Note.h"

namespace GBA
{
	namespace DMG
	{
		namespace SquareSound
		{
			enum WaveDutyCycle
			{
				Cycle_1_8,	// 1/8
				Cycle_1_4,	// 1/4 
				Cycle_1_2,	// 1/2
				Cycle_3_4,	// 3/4
			};

			enum EnvelopeStepDirection
			{
				Decrease,
				Increase,
			};

			namespace Frequency
			{
				enum Sustain
				{
					Hold,	// Sound plays forever
					Timed,	// Sound plays for as long as the length field
				};
			}

			namespace Sweep
			{
				enum Mode
				{
					Increase,
					Decrease,
				};
			}
		}

		enum SoundChannels
		{
			Sound1 = BIT(0),		// Square with frequency sweep, which can make the frequency rise or drop exponentially as it's played
			Sound2 = BIT(1),		// Square 
			Sound3 = BIT(2),		// Wave
			Sound4 = BIT(3),		// Noise generator

			SoundChannelsCount = 4
		};

		enum VolumeRatio
		{
			Vol25,
			Vol50,
			Vol100
		};

		/// <summary>
		/// Enabled individual sound channels
		/// </summary>
		/// <param name="soundChannels">Bitmask of enum DMG::SoundChannels (Sound1 | Sound2) etc</param>
		void EnableSoundChannels(int soundChannels);

		/// <summary>
		/// Sets the volume for both the left and right speakers for the dmg channels
		/// </summary>
		/// <param name="volume">Value from 0-1</param>
		void SetMasterVolume(float volume);

		/// <summary>
		/// Sets the volume for individual speakers for the dmg channels
		/// </summary>
		/// <param name="volume">Value from 0-1</param>
		void SetMasterVolume(float leftVolume, float rightVolume);

		void SetVolumeRatio(VolumeRatio vol);

		struct SquareSoundRegister
		{
			// Sound length= (64-register value)*(1/256) seconds
			u16 soundLength : 6		// WRITE-ONLY, only works if the channel is timed (Frequency::Sustain::Timed on FrequencyRegister). The length itself is actually (64-soundLength)/256 seconds for a [3.9, 250] ms range.
				, waveDutyCycle : 2 // Ratio between on and of times of the square wave.
				, envelopeStepTime : 3 // Time between envelope changes: ƒ¢t = EST/64 s.
				, envelopeDirection : 1 // Indicates if the envelope decreases (default/0) or increases (1) with each step.
				, envelopeInitialVolume : 4 // Can be considered a volume setting of sorts: 0 is silent and 15 is full volume. Combined with the direction, you can have fade-in and fade-outs; to have a sustaining sound, set initial volume to 15 and an increasing direction.
				;

			/// <summary>
			/// Converts time to soundLength viable value and sets the soundLength register
			/// </summary>
			/// <param name="milliseconds">Range of [3.9, 250] milliseconds</param>
			void SetSoundLengthSeconds(float seconds);
		};

		// Frequency sweeps allows "portamento"-like effects where the frequency raises or decreases during playback
		struct FrequencyRegister
		{
			u16 soundRate : 11 // soundRate = 2^11 - 2^17 / frequency, 64Hz - 131Khz. Write-only field.
				, : 3
				, sustain : 1	// If set, the sound plays for as long as the length field (SquareSoundRegister.soundLength) indicates. If clear, the sound plays forever. Note that even if a decaying envelope has reached 0, the sound itself would still be considered on, even if it's silent.
				, reset : 1		// Write-only. Resets the sound to the initial volume (and sweep) settings, i.e use this to play a new sound.
				;
		};

		// The sweep does not apply to the frequency. It does not apply to the period. It applies to FrequencyRegister.soundRate
		struct SweepRegister
		{
			u16 number : 3
				, mode : 1
				, stepTime : 3		// The time between sweeps is measured in 128 Hz (not kHz!): ƒ¢t = T/128 ms ~= 7.8T ms; if T=0, the sweep is disabled.
				;

		};

		struct SoundChannel1
		{
			SweepRegister sweep;
			SquareSoundRegister control;
			FrequencyRegister frequency;
		};

		struct SoundChannel2
		{
			SquareSoundRegister control;
			FrequencyRegister frequency;
		};

		// 64Hz - 131Khz please
		constexpr u32 FrequencyToRate(double frequency)
		{
			return (1 << 11) - u32((1 << 17) / (frequency)+0.5);
		}

		u32 NoteToRate(Music::Note note, int octave);

		void Test();
	}
}
