#include "GBADmgSound.h"
#include "engine/base/Typedefs.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/base/Macros.h"

namespace GBA
{
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
					arr[octave * Music::Note::Count + note] = (u32)(DMG::FrequencyToRate(Music::NoteFrequency((Music::Note)note, octave)));
				}
			}
		}

		constexpr u32 GetRate(Music::Note note, int octave) const
		{
			return arr[octave * Music::Note::Count + note];
		}
	};

	constexpr NoteToRateLookupTable noteLut;

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

	constexpr int DMG_VOL_MAX = 7;

	void DMG::EnableSoundChannels(int soundChannels)
	{
		DmgControlRegister& dmgControlRegister = (*reinterpret_cast<DmgControlRegister*>(REG_SND_DMGCONTROL));

		dmgControlRegister.channelEnabledFlagsL = soundChannels;
		dmgControlRegister.channelEnabledFlagsR = soundChannels;
	}

	void DMG::SetMasterVolume(float volume)
	{
		SetMasterVolume(volume, volume);
	}

	void DMG::SetMasterVolume(float leftVolume, float rightVolume)
	{
		DmgControlRegister& dmgControlRegister = (*reinterpret_cast<DmgControlRegister*>(REG_SND_DMGCONTROL));

		int dmgVolumeValueL = int(leftVolume * DMG_VOL_MAX + 0.5f);
		int dmgVolumeValueR = int(rightVolume * DMG_VOL_MAX + 0.5f);

		dmgControlRegister.masterVolumeL = dmgVolumeValueL;
		dmgControlRegister.masterVolumeR = dmgVolumeValueR;
	}

	void DMG::SetVolumeRatio(VolumeRatio vol)
	{
		DmgDirectSoundControlRegister& dmgDirectSoundControl = (*reinterpret_cast<DmgDirectSoundControlRegister*>(REG_SND_DIRECTSOUNDCONTROL));
		dmgDirectSoundControl.dmgVolumeRatio = vol;
	}

	void DMG::SquareSoundRegister::SetSoundLengthSeconds(float seconds)
	{
		// Sound length= (64-register value)*(1/256) seconds
		int registerValue = 64 - (seconds * 256);
		soundLength = registerValue;
	}

	void DMG::Test()
	{
		DMG::SweepRegister& sound1Sweep = (*reinterpret_cast<DMG::SweepRegister*>(REG_SND1_SWEEP));
		DMG::SquareSoundRegister& sound1Control = (*reinterpret_cast<DMG::SquareSoundRegister*>(REG_SND1_CONTROL));
		DMG::FrequencyRegister& sound1Frequency = (*reinterpret_cast<DMG::FrequencyRegister*>(REG_SND1_FREQ));

		//DMG::SquareSoundRegister& sound2Control = (*reinterpret_cast<DMG::SquareSoundRegister*>(REG_SND2_CONTROL));
		//DMG::FrequencyRegister& sound2Frequency = (*reinterpret_cast<DMG::FrequencyRegister*>(REG_SND2_FREQ));

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

	u32 DMG::NoteToRate(Music::Note note, int octave)
	{
		return noteLut.GetRate(note, octave);
	}
}
