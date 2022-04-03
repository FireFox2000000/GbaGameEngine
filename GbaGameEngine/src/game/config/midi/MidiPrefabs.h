#pragma once
#include "engine/gba/audio/GBADmgSound.h"
#include "engine/gba/audio/GBADmgMidiPlayer.h"

namespace Midi
{
	namespace Prefabs
	{
		template<class Channel>
		Channel OverrideFrequency(Channel prefab, Music::Note note, int octave)
		{
			prefab.frequency.soundRate = GBA::DMG::NoteToRate(note, octave);
			return prefab;
		}

		template<class Channel>
		Channel OverrideEnvelopeDirection(Channel prefab, GBA::DMG::SquareSound::EnvelopeStepDirection dir)
		{
			prefab.control.envelopeDirection = dir;
			prefab.frequency.reset = false;
			return prefab;
		}

		GBA::DMG::SoundChannel1 OverrideSweepMode(GBA::DMG::SoundChannel1 prefab, GBA::DMG::SquareSound::Sweep::Mode mode);

		template<class Channel>
		GBA::DMG::Midi::NoteEvent MakeNoteEvent(u32 deltaTick, Channel properties)
		{
			GBA::DMG::Midi::NoteEvent note;
			note.deltaTick = deltaTick;
			note.SetChannelProperties(properties);
			return note;
		};

		GBA::DMG::SoundChannel2 OffNotePrefab();
		GBA::DMG::SoundChannel4 DrumTomPrefab();
	}
}
