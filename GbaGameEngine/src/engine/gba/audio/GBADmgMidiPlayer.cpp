#include "GBADmgMidiPlayer.h"
#include "engine/gba/registers/RegisterMap.h"

GBA::DMG::Midi::Player::Player(const NoteEvent* begin, const NoteEvent* end)
	: m_file(nullptr)
	, m_fileStream(nullptr)
{
	m_noteEventsBegin = begin;
	m_noteEventsEnd = end;

	DEBUG_ASSERTMSG(begin != end, "Cannot play empty midi file");

	m_tickUntilNextEvent = begin->deltaTick;
}

GBA::DMG::Midi::Player::Player(FilePtr file)
	: m_file(file)
	, m_fileStream(file)
{
	m_totalEvents = m_fileStream.Read<u32>();
	m_tickUntilNextEvent = m_fileStream.Read<u16>();
}

struct FileEventId
{
	u16 channelId : 2
		, dataMask : 14
		;
};

void GBA::DMG::Midi::Player::Tick()
{
	if (m_file)
	{
		m_tickUntilNextEvent = MAX(m_tickUntilNextEvent - 1, 0);

		while (m_eventIndex < m_totalEvents && m_tickUntilNextEvent <= 0)
		{
			FileEventId eventId = m_fileStream.Read<FileEventId>();
			u16 channel = eventId.channelId;
			u16 dataMask = eventId.dataMask;

			switch (channel)
			{
			case 0:
			{
				if (dataMask & BIT(1))
				{
					DMG::SweepRegister& sound1Sweep = (*reinterpret_cast<DMG::SweepRegister*>(REG_SND1_SWEEP));
					sound1Sweep = m_fileStream.Read<DMG::SweepRegister>();
				}

				if (dataMask & BIT(2))
				{
					DMG::SquareSoundRegister& sound1Control = (*reinterpret_cast<DMG::SquareSoundRegister*>(REG_SND1_CONTROL));
					sound1Control = m_fileStream.Read<DMG::SquareSoundRegister>();
				}

				if (dataMask & BIT(3))
				{
					DMG::FrequencyRegister& sound1Frequency = (*reinterpret_cast<DMG::FrequencyRegister*>(REG_SND1_FREQ));
					sound1Frequency = m_fileStream.Read<DMG::FrequencyRegister>();
				}
				break;
			}
			
			case 1:
			{
				if (dataMask & BIT(1))
				{
					DMG::SquareSoundRegister& sound2Control = (*reinterpret_cast<DMG::SquareSoundRegister*>(REG_SND2_CONTROL));
					sound2Control = m_fileStream.Read<DMG::SquareSoundRegister>();
				}

				if (dataMask & BIT(2))
				{
					DMG::FrequencyRegister& sound2Frequency = (*reinterpret_cast<DMG::FrequencyRegister*>(REG_SND2_FREQ));
					sound2Frequency = m_fileStream.Read<DMG::FrequencyRegister>();
				}
				break;
			}
			}

			bool enableLooping = true;
			if (++m_eventIndex >= m_totalEvents && enableLooping)
			{
				// Loop
				m_eventIndex = 0;
				m_fileStream.ResetStreamPos();
				m_totalEvents = m_fileStream.Read<u32>();
			}

			if (m_eventIndex < m_totalEvents)
			{
				m_tickUntilNextEvent = m_fileStream.Read<u16>();
			}
		}
	}
	else
	{
		m_tickUntilNextEvent = MAX(m_tickUntilNextEvent - 1, 0);

		const NoteEvent* currentEvent = m_noteEventsBegin + m_eventIndex;
		while (currentEvent != m_noteEventsEnd && m_tickUntilNextEvent <= 0)	// We may need to play multiple note events at the same time
		{
			OnNoteEventReached(*currentEvent);
			currentEvent = m_noteEventsBegin + m_eventIndex;
		}
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

	case DMG::SoundChannels::Sound4:
	{
		auto& sound4Control = (*reinterpret_cast<DMG::NoiseControlRegister*>(REG_SND4_CONTROL));
		auto& sound4Frequency = (*reinterpret_cast<DMG::NoiseFrequencyRegister*>(REG_SND4_FREQ));

		const auto& channel = noteEvent.channel.channel4;
		sound4Control = channel.control;
		sound4Frequency = channel.frequency;
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

void GBA::DMG::Midi::NoteEvent::SetChannelProperties(const SoundChannel4& properties)
{
	channelId = DMG::SoundChannels::Sound4;
	channel.channel4 = properties;
}
