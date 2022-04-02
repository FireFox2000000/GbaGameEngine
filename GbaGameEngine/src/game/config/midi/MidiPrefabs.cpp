#include "MidiPrefabs.h"

GBA::DMG::SoundChannel1 Midi::Prefabs::OverrideFrequency(GBA::DMG::SoundChannel1 prefab, Music::Note note, int octave)
{
	prefab.frequency.soundRate = GBA::DMG::NoteToRate(note, octave);
	return prefab;
}

GBA::DMG::SoundChannel2 Midi::Prefabs::OverrideFrequency(GBA::DMG::SoundChannel2 prefab, Music::Note note, int octave)
{
	prefab.frequency.soundRate = GBA::DMG::NoteToRate(note, octave);
	return prefab;
};

GBA::DMG::Midi::NoteEvent Midi::Prefabs::MakeNoteEvent(u32 deltaTick, GBA::DMG::SoundChannel1 properties)
{
	GBA::DMG::Midi::NoteEvent note;
	note.deltaTick = deltaTick;
	note.SetChannelProperties(properties);
	return note;
};

GBA::DMG::Midi::NoteEvent Midi::Prefabs::MakeNoteEvent(u32 deltaTick, GBA::DMG::SoundChannel2 properties)
{
	GBA::DMG::Midi::NoteEvent note;
	note.deltaTick = deltaTick;
	note.SetChannelProperties(properties);
	return note;
};

GBA::DMG::SoundChannel2 Midi::Prefabs::OffNotePrefab()
{
	GBA::DMG::SoundChannel2 noteOff;
	noteOff.control.envelopeStepTime = 3;
	noteOff.control.envelopeDirection = GBA::DMG::SquareSound::EnvelopeStepDirection::Decrease;
	noteOff.control.envelopeInitialVolume = 0;

	noteOff.frequency.sustain = GBA::DMG::SquareSound::Frequency::Sustain::Hold;
	noteOff.frequency.reset = true;

	return noteOff;
}

GBA::DMG::SoundChannel2 Midi::Prefabs::DrumTomPrefab()
{
	GBA::DMG::SoundChannel2 note;

	//note.sweep.mode = GBA::DMG::SquareSound::Sweep::Mode::Decrease;
	//note.sweep.number = 7;
	//note.sweep.stepTime = 7;

	note.control.envelopeStepTime = 3;
	note.control.envelopeDirection = GBA::DMG::SquareSound::EnvelopeStepDirection::Decrease;
	note.control.envelopeInitialVolume = 15;
	note.control.waveDutyCycle = GBA::DMG::SquareSound::WaveDutyCycle::Cycle_3_4;
	note.control.soundLength = 60;

	note.frequency.soundRate = GBA::DMG::NoteToRate(Music::Note::G, 3);
	note.frequency.sustain = GBA::DMG::SquareSound::Frequency::Sustain::Timed;
	note.frequency.reset = true;

	return note;
}
