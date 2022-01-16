#include "GBADmgSound.h"
#include "engine/base/Typedefs.h"
#include "engine/audio/music/Note.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/base/Macros.h"

namespace GBA
{
	// 64Hz - 131Khz please
	constexpr u32 FrequencyToRate(double frequency)
	{
		return (1 << 11) - u32((1 << 17) / (frequency)+0.5);
	}

	const int MAX_OCTIVES = 9;

	class NoteToRateLookupTable {
		u32 arr[Music::Note::Count * MAX_OCTIVES];

	public:
		constexpr NoteToRateLookupTable() : arr()
		{
			for (int octave = 0; octave < MAX_OCTIVES; ++octave)
			{
				for (int note = 0; note < Music::Note::Count; ++note)
				{
					arr[octave * Music::Note::Count + note] = (u32)(FrequencyToRate(Music::NoteFrequency((Music::Note)note, octave)));
				}
			}
		}

		constexpr u32 GetRate(Music::Note note, int octave)
		{
			return arr[octave * Music::Note::Count + note];
		}
	};

	constexpr NoteToRateLookupTable noteLut;

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
		void SetSoundLengthSeconds(float seconds)
		{
			// Sound length= (64-register value)*(1/256) seconds
			int registerValue = 64 - (seconds * 256);
			soundLength = registerValue;
		}
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

	struct DmgControlRegister
	{
		u16 masterVolumeR : 3		// (0-7)
			, : 1
			, masterVolumeL : 3		// (0-7)
			, : 1
			, channelEnabledFlagsR : 4
			, channelEnabledFlagsL : 4
			;
	};

	struct DmgDirectSoundControlRegister
	{
		u16 dmgVolumeRatio : 2			// As to why there's DMG bits in direct sound control registers... who knows???
			;
	};

	DmgControlRegister& dmgControlRegister = (*reinterpret_cast<DmgControlRegister*>(REG_SND_DMGCONTROL));
	DmgDirectSoundControlRegister& dmgDirectSoundControl = (*reinterpret_cast<DmgDirectSoundControlRegister*>(REG_SND_DIRECTSOUNDCONTROL));

	SquareSoundRegister& sound1Control = (*reinterpret_cast<SquareSoundRegister*>(REG_SND1_CONTROL));
	FrequencyRegister& sound1Frequency = (*reinterpret_cast<FrequencyRegister*>(REG_SND1_FREQ));
	SweepRegister& sound1Sweep = (*reinterpret_cast<SweepRegister*>(REG_SND1_SWEEP));

	SquareSoundRegister& sound2Control = (*reinterpret_cast<SquareSoundRegister*>(REG_SND2_CONTROL));
	FrequencyRegister& sound2Frequency = (*reinterpret_cast<FrequencyRegister*>(REG_SND2_FREQ));

	constexpr int DMG_VOL_MAX = 7;

	void DMG::EnableSoundChannels(int soundChannels)
	{
		dmgControlRegister.channelEnabledFlagsL = soundChannels;
		dmgControlRegister.channelEnabledFlagsR = soundChannels;
	}

	void DMG::SetMasterVolume(float volume)
	{
		SetMasterVolume(volume, volume);
	}

	void DMG::SetMasterVolume(float leftVolume, float rightVolume)
	{
		int dmgVolumeValueL = int(leftVolume * DMG_VOL_MAX + 0.5f);
		int dmgVolumeValueR = int(rightVolume * DMG_VOL_MAX + 0.5f);

		dmgControlRegister.masterVolumeL = dmgVolumeValueL;
		dmgControlRegister.masterVolumeR = dmgVolumeValueR;
	}

	void DMG::SetVolumeRatio(VolumeRatio vol)
	{
		dmgDirectSoundControl.dmgVolumeRatio = vol;
	}

	void DMG::Test()
	{
		EnableSoundChannels(BIT(0));
		SetMasterVolume(1.0f);

		// NR10_REG = 0x1E;
		sound1Sweep.number = 6;
		sound1Sweep.mode = DMG::SquareSound::Sweep::Mode::Decrease;
		sound1Sweep.stepTime = 1;

		// NR11_REG = 0x10;
		// NR12_REG = 0xF3;
		sound1Control.SetSoundLengthSeconds(0.1875f);	// sound1Control.soundLength = 16;
		sound1Control.envelopeStepTime = 3;
		sound1Control.envelopeDirection = DMG::SquareSound::EnvelopeStepDirection::Increase;
		sound1Control.envelopeInitialVolume = 7;

		// NR13_REG = 0x00;
		// NR14_REG = 0x87;
		sound1Frequency.soundRate = FrequencyToRate(512);
		sound1Frequency.sustain = DMG::SquareSound::Frequency::Sustain::Timed;
		sound1Frequency.reset = true;
	}
}
