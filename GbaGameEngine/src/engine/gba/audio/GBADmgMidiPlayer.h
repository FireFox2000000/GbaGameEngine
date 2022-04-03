#pragma once
#include "GBADmgSound.h"
#include "engine/io/File.h"
#include "engine/io/filestream/CppFileReader.h"

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
				SoundChannel4 channel4;
			};

			struct NoteEvent
			{
				u32 deltaTick;
				DMG::SoundChannels channelId;
				USoundChannel channel;

				void SetChannelProperties(const SoundChannel1& properties);
				void SetChannelProperties(const SoundChannel2& properties);
				void SetChannelProperties(const SoundChannel4& properties);
			};

			class Player
			{
			private:
				int m_tickUntilNextEvent;
				u32 m_eventIndex = 0;
				const NoteEvent* m_noteEventsBegin;
				const NoteEvent* m_noteEventsEnd;

				FilePtr m_file;
				CppFileReader m_fileStream;
				u32 m_totalEvents = 0;

				void OnNoteEventReached(const NoteEvent& noteEvent);

			public:
				Player(const NoteEvent* begin, const NoteEvent* end);
				Player(FilePtr file);
				void Tick();
			};
		}
	}
}