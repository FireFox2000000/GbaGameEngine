#pragma once
#include "engine/gba/audio/GBADmgSound.h"
#include "engine/gba/audio/GBADmgMidiPlayer.h"

namespace Midi
{
	namespace Prefabs
	{
		GBA::DMG::SoundChannel1 OverrideFrequency(GBA::DMG::SoundChannel1 prefab, Music::Note note, int octave);
		GBA::DMG::SoundChannel2 OverrideFrequency(GBA::DMG::SoundChannel2 prefab, Music::Note note, int octave);
		GBA::DMG::Midi::NoteEvent MakeNoteEvent(u32 deltaTick, GBA::DMG::SoundChannel1 properties);
		GBA::DMG::Midi::NoteEvent MakeNoteEvent(u32 deltaTick, GBA::DMG::SoundChannel2 properties);

		GBA::DMG::SoundChannel2 OffNotePrefab();
		GBA::DMG::SoundChannel2 DrumTomPrefab();
	}
}
