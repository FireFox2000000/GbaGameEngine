#include "MidiFallOfFall.h"
#include "MidiPrefabs.h"
#include "engine/base/Macros.h"
#include "engine/gba/audio/GBADmgSound.h"

using namespace Midi::Prefabs;

GBA::DMG::Midi::SoundChannel1Prefab HeldNotePrefab();

GBA::DMG::Midi::SoundChannel2Prefab HeldNotePrefab2()
{
	GBA::DMG::Midi::SoundChannel2Prefab heldNote;

	heldNote.squareSound.envelopeStepTime = 3;
	heldNote.squareSound.envelopeDirection = GBATEK::EnvelopeStepDirection::Decrease;
	heldNote.squareSound.envelopeInitialVolume = 15;
	heldNote.squareSound.waveDutyCycle = GBATEK::WaveDutyCycle::Cycle_3_4;

	heldNote.frequency.soundRate = GBA::DMG::NoteToRate(Music::Note::G, 6);// FrequencyToRate(512);
	heldNote.frequency.sustainMode = GBATEK::SustainMode::Forever;
	heldNote.frequency.reset = true;

	return heldNote;
}

auto HeldNote = HeldNotePrefab();
auto HeldNote2 = HeldNotePrefab2();

auto OffNote = OffNotePrefab();
auto DrumTom = DrumTomPrefab();

// BPM definitions
const int TickStep8 = 15;
const int TickStep4 = 30;
const int TickStep2 = 60;

auto VibratoNote = OverrideFrequency(HeldNote, Music::Note::G, 5);

const GBA::DMG::Midi::NoteEvent NoteEvents[] = {
	/*
	// Drum beat test
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, DrumTom),
	MakeNoteEvent(TickStep8, OffNote),
	
	// Melody test
	MakeNoteEvent(TickStep8,	OverrideFrequency(HeldNote, Music::Note::G,	5)),
	MakeNoteEvent(TickStep8,	OverrideFrequency(HeldNote, Music::Note::A,	5)),
	MakeNoteEvent(TickStep8,	OverrideFrequency(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep8,	OverrideFrequency(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep4,	OverrideFrequency(HeldNote, Music::Note::C,	6)),
	MakeNoteEvent(TickStep4,	OverrideFrequency(HeldNote, Music::Note::D,	6)),
	MakeNoteEvent(TickStep4,	OverrideFrequency(HeldNote, Music::Note::C,	6)),
	MakeNoteEvent(TickStep4,	OverrideFrequency(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep8,	OverrideFrequency(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep2,	OverrideFrequency(HeldNote, Music::Note::G,	5)),
	MakeNoteEvent(TickStep8,	OverrideFrequency(HeldNote, Music::Note::A,	5)),
	MakeNoteEvent(TickStep8,	OverrideFrequency(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep8,	OverrideFrequency(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep4,	OverrideFrequency(HeldNote, Music::Note::G,	6)),
	MakeNoteEvent(TickStep4,	OverrideFrequency(HeldNote, Music::Note::Fs,6)),
	MakeNoteEvent(TickStep4,	OverrideFrequency(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep2, OffNote),
	*/
	// Vibrato Test
	MakeNoteEvent(TickStep8,	VibratoNote),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Increase)),
	MakeNoteEvent(2,	OverrideSweepMode(VibratoNote, GBATEK::SweepFrequencyDirection::Decrease)),
	MakeNoteEvent(TickStep2, OffNote),
	
};

/***********************************************************************************************/

const GBA::DMG::Midi::NoteEvent* Midi::FallOfFall::begin()
{
	return &NoteEvents[0];
}

const GBA::DMG::Midi::NoteEvent* Midi::FallOfFall::end()
{
	constexpr int TotalEvents = ARRAY_SIZE(NoteEvents);
	return begin() + TotalEvents;
}

GBA::DMG::Midi::SoundChannel1Prefab HeldNotePrefab()
{
	GBA::DMG::Midi::SoundChannel1Prefab heldNote;

	heldNote.sweep.mode = GBATEK::SweepFrequencyDirection::Decrease;
	heldNote.sweep.number = 7;
	heldNote.sweep.stepTime128Hz = 1;

	heldNote.squareSound.envelopeStepTime = 7;
	heldNote.squareSound.envelopeDirection = GBATEK::EnvelopeStepDirection::Decrease;
	heldNote.squareSound.envelopeInitialVolume = 15;
	heldNote.squareSound.waveDutyCycle = GBATEK::WaveDutyCycle::Cycle_3_4;

	heldNote.frequency.soundRate = GBA::DMG::NoteToRate(Music::Note::G, 6);// FrequencyToRate(512);
	heldNote.frequency.sustainMode = GBATEK::SustainMode::Forever;
	heldNote.frequency.reset = true;

	return heldNote;
}

