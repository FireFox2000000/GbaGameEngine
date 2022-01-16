#pragma once
#include <cmath>

namespace Music
{
	enum Note
	{
		C,
		Cs,
		D,
		Ds,
		E,
		F,
		Fs,
		G,
		Gs,
		A,
		As,
		B,

		Count,

		Df = Cs,
		Ef = Ds,
		Gf = Fs,
		Af = Gs,
		Bf = As,
	};

	// If we really need to call this dynamically it should be looked up from a lut, as this function is very slow on gba hardware.
	constexpr double NoteFrequency(Note note, int octave)
	{
		// http://techlib.com/reference/musical_note_frequencies.htm
		// Notes are separated by a factor of 2^(1/12) or 1.059463.
		const double C0Freq = 16.35160;
		int numberOfNotesAwayFromBase = octave * Music::Note::Count + note;
		return C0Freq * pow(2.0, (numberOfNotesAwayFromBase / 12.00));
	}
}
