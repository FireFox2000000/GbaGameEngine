#pragma once
#include "engine/base/Macros.h"

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

		void Test();
	}
}
