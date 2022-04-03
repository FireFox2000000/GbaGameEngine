#include "MidiPrefabs.h"

GBA::DMG::SoundChannel1 Midi::Prefabs::OverrideSweepMode(GBA::DMG::SoundChannel1 prefab, GBA::DMG::SquareSound::Sweep::Mode mode)
{
	prefab.sweep.mode = mode;
	prefab.frequency.reset = false;
	return prefab;
}

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

GBA::DMG::SoundChannel4 Midi::Prefabs::DrumTomPrefab()
{
	GBA::DMG::SoundChannel4 note;

	note.control.envelopeStepTime = 6;
	note.control.envelopeDirection = GBA::DMG::SquareSound::EnvelopeStepDirection::Decrease;
	note.control.envelopeInitialVolume = 15;
	note.control.soundLength = 60;

	note.frequency.frequencyDividerRatio = 6;
	note.frequency.counterStep = GBA::DMG::Noise::CounterStep::Step7;
	note.frequency.shiftClockFreq = 2;
	note.frequency.sustain = GBA::DMG::SquareSound::Frequency::Sustain::Timed;
	note.frequency.reset = true;

	return note;
}
