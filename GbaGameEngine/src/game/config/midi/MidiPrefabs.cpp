#include "MidiPrefabs.h"

GBA::DMG::Midi::SoundChannel1Prefab Midi::Prefabs::OverrideSweepMode(GBA::DMG::Midi::SoundChannel1Prefab prefab, GBATEK::SweepFrequencyDirection mode)
{
	prefab.sweep.mode = mode;
	prefab.frequency.reset = GBATEK::SoundReset::NoReset;
	return prefab;
}

GBA::DMG::Midi::SoundChannel2Prefab Midi::Prefabs::OffNotePrefab()
{
	GBA::DMG::Midi::SoundChannel2Prefab noteOff;
	noteOff.squareSound.envelopeStepTime = 3;
	noteOff.squareSound.envelopeDirection = GBATEK::EnvelopeStepDirection::Decrease;
	noteOff.squareSound.envelopeInitialVolume = 0;

	noteOff.frequency.sustainMode = GBATEK::SustainMode::Forever;
	noteOff.frequency.reset = GBATEK::SoundReset::Reset;

	return noteOff;
}

GBA::DMG::Midi::SoundChannel4Prefab Midi::Prefabs::DrumTomPrefab()
{
	GBA::DMG::Midi::SoundChannel4Prefab note;

	note.control.envelopeStepTime = 6;
	note.control.envelopeDirection = GBATEK::EnvelopeStepDirection::Decrease;
	note.control.envelopeInitialVolume = 15;
	note.control.soundLength = 60;

	note.frequency.frequencyDividerRatio = 6;
	note.frequency.counterStep = GBATEK::CounterStep::Step7;
	note.frequency.shiftClockFreq = 2;
	note.frequency.sustainMode = GBATEK::SustainMode::SoundLength;
	note.frequency.reset = GBATEK::SoundReset::Reset;

	return note;
}
