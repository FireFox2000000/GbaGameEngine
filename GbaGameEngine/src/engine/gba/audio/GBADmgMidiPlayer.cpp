#include "GBADmgMidiPlayer.h"
#include "engine/gba/registers/RegisterMap.h"

GBA::DMG::Midi::Player::Player(const NoteEvent* begin, const NoteEvent* end)
{
	m_noteEventsBegin = begin;
	m_noteEventsEnd = end;

	DEBUG_ASSERTMSG(begin != end, "Cannot play empty midi file");

	m_tickUntilNextEvent = begin->deltaTick;
}

void GBA::DMG::Midi::Player::Tick()
{
	m_tickUntilNextEvent = MAX(m_tickUntilNextEvent - 1, 0);

	const NoteEvent* currentEvent = m_noteEventsBegin + m_eventIndex;
	while (currentEvent != m_noteEventsEnd && m_tickUntilNextEvent <= 0)	// We may need to play multiple note events at the same time
	{
		OnNoteEventReached(*currentEvent);
		currentEvent = m_noteEventsBegin + m_eventIndex;
	}
}

void GBA::DMG::Midi::Player::OnNoteEventReached(const NoteEvent& noteEvent)
{
	switch (noteEvent.channelId)
	{
	case DMG::SoundChannels::Sound1:
	{
		DMG::SweepRegister& sound1Sweep = (*reinterpret_cast<DMG::SweepRegister*>(REG_SND1_SWEEP));
		DMG::SquareSoundRegister& sound1Control = (*reinterpret_cast<DMG::SquareSoundRegister*>(REG_SND1_CONTROL));
		DMG::FrequencyRegister& sound1Frequency = (*reinterpret_cast<DMG::FrequencyRegister*>(REG_SND1_FREQ));

		const auto& channel = noteEvent.channel.channel1;
		sound1Sweep = channel.sweep;
		sound1Control = channel.control;
		sound1Frequency = channel.frequency;

		break;
	}

	case DMG::SoundChannels::Sound2:
	{
		DMG::SquareSoundRegister& sound2Control = (*reinterpret_cast<DMG::SquareSoundRegister*>(REG_SND2_CONTROL));
		DMG::FrequencyRegister& sound2Frequency = (*reinterpret_cast<DMG::FrequencyRegister*>(REG_SND2_FREQ));

		const auto& channel = noteEvent.channel.channel2;
		sound2Control = channel.control;
		sound2Frequency = channel.frequency;
		break;
	}

	default: break;
	}

	const NoteEvent* currentEvent = m_noteEventsBegin + ++m_eventIndex;

	if (currentEvent != m_noteEventsEnd)
	{
		m_tickUntilNextEvent += currentEvent->deltaTick;
	}
	else
	{
		m_tickUntilNextEvent = 1;
	}
}

void GBA::DMG::Midi::NoteEvent::SetChannelProperties(const SoundChannel1& properties)
{
	channelId = DMG::SoundChannels::Sound1;
	channel.channel1 = properties;
}

void GBA::DMG::Midi::NoteEvent::SetChannelProperties(const SoundChannel2& properties)
{
	channelId = DMG::SoundChannels::Sound2;
	channel.channel2 = properties;
}
