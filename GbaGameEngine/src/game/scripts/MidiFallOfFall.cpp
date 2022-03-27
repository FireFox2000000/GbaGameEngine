#include "MidiFallOfFall.h"
#include "engine/gba/audio/GBADmgSound.h"
#include "engine/gba/audio/GBADmgMidiPlayer.h"

GBA::DMG::SoundChannel2 HeldNotePrefab();
GBA::DMG::SoundChannel2 OffNotePrefab();
GBA::DMG::SoundChannel2 MakeChannelProperties(GBA::DMG::SoundChannel2 prefab, Music::Note note, int octave);
GBA::DMG::Midi::NoteEvent MakeNoteEvent(u32 deltaTick, GBA::DMG::SoundChannel2 properties);

GBA::DMG::SoundChannel2 HeldNote = HeldNotePrefab();
GBA::DMG::SoundChannel2 OffNote = OffNotePrefab();

// BPM definitions
const int TickStep8 = 15;
const int TickStep4 = 30;
const int TickStep2 = 60;

const GBA::DMG::Midi::NoteEvent NoteEvents[] = {
	MakeNoteEvent(TickStep8, MakeChannelProperties(HeldNote, Music::Note::G,	5)),
	MakeNoteEvent(TickStep8, MakeChannelProperties(HeldNote, Music::Note::A,	5)),
	MakeNoteEvent(TickStep8, MakeChannelProperties(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep8, MakeChannelProperties(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep4, MakeChannelProperties(HeldNote, Music::Note::C,	6)),
	MakeNoteEvent(TickStep4, MakeChannelProperties(HeldNote, Music::Note::D,	6)),
	MakeNoteEvent(TickStep4, MakeChannelProperties(HeldNote, Music::Note::C,	6)),
	MakeNoteEvent(TickStep4, MakeChannelProperties(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep8, MakeChannelProperties(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep2, MakeChannelProperties(HeldNote, Music::Note::G,	5)),
	MakeNoteEvent(TickStep8, MakeChannelProperties(HeldNote, Music::Note::A,	5)),
	MakeNoteEvent(TickStep8, MakeChannelProperties(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep8, MakeChannelProperties(HeldNote, Music::Note::B,	5)),
	MakeNoteEvent(TickStep4, MakeChannelProperties(HeldNote, Music::Note::G,	6)),
	MakeNoteEvent(TickStep4, MakeChannelProperties(HeldNote, Music::Note::Fs,	6)),
	MakeNoteEvent(TickStep4, MakeChannelProperties(HeldNote, Music::Note::B,	5)),
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


GBA::DMG::SoundChannel2 MakeChannelProperties(GBA::DMG::SoundChannel2 prefab, Music::Note note, int octave)
{
	prefab.frequency.soundRate = GBA::DMG::NoteToRate(note, octave);
	return prefab;
};

GBA::DMG::Midi::NoteEvent MakeNoteEvent(u32 deltaTick, GBA::DMG::SoundChannel2 properties)
{
	GBA::DMG::Midi::NoteEvent note;
	note.deltaTick = deltaTick;
	note.SetChannelProperties(properties);
	return note;
};

GBA::DMG::SoundChannel2 HeldNotePrefab()
{
	GBA::DMG::SoundChannel2 heldNote;
	heldNote.control.envelopeStepTime = 3;
	heldNote.control.envelopeDirection = GBA::DMG::SquareSound::EnvelopeStepDirection::Decrease;
	heldNote.control.envelopeInitialVolume = 15;

	heldNote.frequency.soundRate = GBA::DMG::NoteToRate(Music::Note::G, 6);// FrequencyToRate(512);
	heldNote.frequency.sustain = GBA::DMG::SquareSound::Frequency::Sustain::Hold;
	heldNote.frequency.reset = true;

	return heldNote;
}

GBA::DMG::SoundChannel2 OffNotePrefab()
{
	GBA::DMG::SoundChannel2 noteOff;
	noteOff.control.envelopeStepTime = 3;
	noteOff.control.envelopeDirection = GBA::DMG::SquareSound::EnvelopeStepDirection::Decrease;
	noteOff.control.envelopeInitialVolume = 0;

	noteOff.frequency.sustain = GBA::DMG::SquareSound::Frequency::Sustain::Hold;
	noteOff.frequency.reset = true;

	return noteOff;
}
