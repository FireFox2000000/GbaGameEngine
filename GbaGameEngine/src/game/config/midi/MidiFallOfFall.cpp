#include "MidiFallOfFall.h"
#include "MidiPrefabs.h"

using namespace Midi::Prefabs;

GBA::DMG::SoundChannel1 HeldNotePrefab();

GBA::DMG::SoundChannel2 HeldNotePrefab2()
{
	GBA::DMG::SoundChannel2 heldNote;

	heldNote.control.envelopeStepTime = 3;
	heldNote.control.envelopeDirection = GBA::DMG::SquareSound::EnvelopeStepDirection::Decrease;
	heldNote.control.envelopeInitialVolume = 15;
	heldNote.control.waveDutyCycle = GBA::DMG::SquareSound::WaveDutyCycle::Cycle_3_4;

	heldNote.frequency.soundRate = GBA::DMG::NoteToRate(Music::Note::G, 6);// FrequencyToRate(512);
	heldNote.frequency.sustain = GBA::DMG::SquareSound::Frequency::Sustain::Hold;
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

const GBA::DMG::Midi::NoteEvent NoteEvents[] = {
	//MakeNoteEvent(TickStep8, DrumTom),
	//MakeNoteEvent(TickStep8, DrumTom),
	//MakeNoteEvent(TickStep8, DrumTom),
	//MakeNoteEvent(TickStep8, DrumTom),
	//MakeNoteEvent(TickStep4, DrumTom),
	//MakeNoteEvent(TickStep4, DrumTom),
	//MakeNoteEvent(TickStep4, DrumTom),
	//MakeNoteEvent(TickStep4, DrumTom),
	//MakeNoteEvent(TickStep8, DrumTom),
	//MakeNoteEvent(TickStep2, DrumTom),
	//MakeNoteEvent(TickStep8, DrumTom),
	//MakeNoteEvent(TickStep8, DrumTom),
	//MakeNoteEvent(TickStep8, DrumTom),
	//MakeNoteEvent(TickStep4, DrumTom),
	//MakeNoteEvent(TickStep4, DrumTom),
	//MakeNoteEvent(TickStep4, DrumTom),
	//MakeNoteEvent(TickStep2, OffNote),
	
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
	
};

/***********************************************************************************************/

const GBA::DMG::Midi::NoteEvent* Midi::FallOfFall::begin()
{
	return &NoteEvents[0];
}

const GBA::DMG::Midi::NoteEvent* Midi::FallOfFall::end()
{
	constexpr int TotalEvents = sizeof(NoteEvents) / sizeof(*NoteEvents);
	return begin() + TotalEvents;
}

GBA::DMG::SoundChannel1 HeldNotePrefab()
{
	GBA::DMG::SoundChannel1 heldNote;

	heldNote.sweep.mode = GBA::DMG::SquareSound::Sweep::Mode::Decrease;
	heldNote.sweep.number = 0;
	heldNote.sweep.stepTime = 0;

	heldNote.control.envelopeStepTime = 3;
	heldNote.control.envelopeDirection = GBA::DMG::SquareSound::EnvelopeStepDirection::Decrease;
	heldNote.control.envelopeInitialVolume = 15;
	heldNote.control.waveDutyCycle = GBA::DMG::SquareSound::WaveDutyCycle::Cycle_3_4;

	heldNote.frequency.soundRate = GBA::DMG::NoteToRate(Music::Note::G, 6);// FrequencyToRate(512);
	heldNote.frequency.sustain = GBA::DMG::SquareSound::Frequency::Sustain::Hold;
	heldNote.frequency.reset = true;

	return heldNote;
}

