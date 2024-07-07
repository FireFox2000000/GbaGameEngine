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

	u32 DMG::NoteToRate(Music::Note note, int octave)
	{
		return noteLut.GetRate(note, octave);
	}
}
