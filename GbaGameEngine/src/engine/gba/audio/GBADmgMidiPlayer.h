#pragma once
#include "GBADmgSound.h"

namespace GBA
{
	namespace DMG
	{
		namespace Midi
		{
			union USoundChannel
			{
				SoundChannel1 channel1;
				SoundChannel2 channel2;
			};

			struct NoteEvent
			{
				u32 deltaTick;
				DMG::SoundChannels channelId;
				USoundChannel channel;

				void SetChannelProperties(const SoundChannel1& properties);

				void SetChannelProperties(const SoundChannel2& properties);
			};

			class Player
			{
			private:
				int m_tickUntilNextEvent;
				u32 m_eventIndex = 0;
				const NoteEvent* m_noteEventsBegin;
				const NoteEvent* m_noteEventsEnd;

				void OnNoteEventReached(const NoteEvent& noteEvent);

			public:
				Player(const NoteEvent* begin, const NoteEvent* end);
				void Tick();
			};
		}
	}
}