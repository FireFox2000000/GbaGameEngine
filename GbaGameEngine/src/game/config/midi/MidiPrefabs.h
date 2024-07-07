#pragma once
#include "GBATEK/DMGSound.h"
#include "engine/audio/music/Note.h"
#include "engine/gba/audio/GBADmgMidiPlayer.h"
#include "engine/gba/audio/GBADmgSound.h"

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
		Channel OverrideEnvelopeDirection(Channel prefab, GBATEK::EnvelopeStepDirection dir)
		{
			prefab.control.envelopeDirection = dir;
			prefab.frequency.reset = false;
			return prefab;
		}

		GBA::DMG::Midi::SoundChannel1Prefab OverrideSweepMode(GBA::DMG::Midi::SoundChannel1Prefab prefab, GBATEK::SweepFrequencyDirection mode);

		template<class Channel>
		GBA::DMG::Midi::NoteEvent MakeNoteEvent(u32 deltaTick, Channel properties)
		{
			GBA::DMG::Midi::NoteEvent note;
			note.deltaTick = deltaTick;
			note.SetChannelProperties(properties);
			return note;
		};

		GBA::DMG::Midi::SoundChannel2Prefab OffNotePrefab();
		GBA::DMG::Midi::SoundChannel4Prefab DrumTomPrefab();
	}
}
