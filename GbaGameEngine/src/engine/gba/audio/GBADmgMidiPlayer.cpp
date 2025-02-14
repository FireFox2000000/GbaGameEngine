#include "GBADmgMidiPlayer.h"
#include "engine/base/Macros.h"
#include "engine/math/Math.h"

GBA::DMG::Midi::Player::Player(const NoteEvent* begin, const NoteEvent* end)
	: m_file(nullptr)
	, m_fileStream(nullptr)
{
	m_noteEventsBegin = begin;
	m_noteEventsEnd = end;

	DEBUG_ASSERTMSG(begin != end, "Cannot play empty midi file");

	m_tickUntilNextEvent = begin->deltaTick;
}

GBA::DMG::Midi::Player::Player(MemoryMappedFileView file)
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
		m_tickUntilNextEvent = Math::Max(m_tickUntilNextEvent - 1, 0);

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
					GBATEK::ioRegisterSoundChannel1->sweep = m_fileStream.Read<GBATEK::SweepSound>();
				}

				if (dataMask & BIT(2))
				{
					GBATEK::ioRegisterSoundChannel1->controlRegister.Write(m_fileStream.Read<GBATEK::SquareSound>());
				}

				if (dataMask & BIT(3))
				{
					GBATEK::ioRegisterSoundChannel1->frequencyRegister.Write(m_fileStream.Read<GBATEK::SoundFrequency>());
				}
				break;
			}
			
			case 1:
			{
				if (dataMask & BIT(1))
				{
					GBATEK::ioRegisterSoundChannel2->controlRegister.Write(m_fileStream.Read<GBATEK::SquareSound>());
				}

				if (dataMask & BIT(2))
				{
					GBATEK::ioRegisterSoundChannel2->frequencyRegister.Write(m_fileStream.Read<GBATEK::SoundFrequency>());
				}
				break;
			}
			}

			if (++m_eventIndex < m_totalEvents)
			{
				m_tickUntilNextEvent = m_fileStream.Read<u16>();
			}
		}
	}
	else
	{
		m_tickUntilNextEvent = Math::Max(m_tickUntilNextEvent - 1, 0);

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
	case 1:
	{
		GBATEK::ioRegisterSoundChannel1->sweep = noteEvent.channel.channel1.sweep;
		GBATEK::ioRegisterSoundChannel1->controlRegister.Write(noteEvent.channel.channel1.squareSound);
		GBATEK::ioRegisterSoundChannel1->frequencyRegister.Write(noteEvent.channel.channel1.frequency);
		break;
	}

	case 2:
	{
		GBATEK::ioRegisterSoundChannel2->controlRegister.Write(noteEvent.channel.channel2.squareSound);
		GBATEK::ioRegisterSoundChannel2->frequencyRegister.Write(noteEvent.channel.channel2.frequency);
		break;
	}

	case 4:
	{
		GBATEK::ioRegisterSoundChannel4->controlRegister.Write(noteEvent.channel.channel4.control);
		GBATEK::ioRegisterSoundChannel4->frequencyRegister.Write(noteEvent.channel.channel4.frequency);
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

void GBA::DMG::Midi::NoteEvent::SetChannelProperties(const SoundChannel1Prefab& properties)
{
	channelId = 1;
	channel.channel1 = properties;
}

void GBA::DMG::Midi::NoteEvent::SetChannelProperties(const SoundChannel2Prefab& properties)
{
	channelId = 2;
	channel.channel2 = properties;
}

void GBA::DMG::Midi::NoteEvent::SetChannelProperties(const SoundChannel4Prefab& properties)
{
	channelId = 4;
	channel.channel4 = properties;
}
