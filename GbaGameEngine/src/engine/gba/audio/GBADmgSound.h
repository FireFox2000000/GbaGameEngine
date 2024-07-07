#pragma once
#include "engine/base/Macros.h"
#include "engine/base/Typedefs.h"
#include "engine/audio/music/Note.h"

namespace GBA
{
	namespace DMG
	{
		// 64Hz - 131Khz please
		constexpr u32 FrequencyToRate(double frequency)
		{
			return (1 << 11) - u32((1 << 17) / (frequency)+0.5);
		}

		u32 NoteToRate(Music::Note note, int octave);

		constexpr unsigned short CalculateSoundLengthSeconds(float seconds)
		{
			// Sound length= (64-register value)*(1/256) seconds
			return 64 - (seconds * 256);
		}
	}
}
