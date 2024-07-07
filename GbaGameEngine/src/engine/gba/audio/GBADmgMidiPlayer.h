#pragma once
#include "GBATEK/DMGSound.h"
#include "engine/io/File.h"
#include "engine/io/filestream/CppFileReader.h"

namespace GBA
{
	namespace DMG
	{
		namespace Midi
		{
			struct SoundChannel1Prefab
			{
				GBATEK::SweepSound sweep;
				GBATEK::SquareSound squareSound;
				GBATEK::SoundFrequency frequency;
			};

			struct SoundChannel2Prefab
			{
				GBATEK::SquareSound squareSound;
				GBATEK::SoundFrequency frequency;
			};

			struct SoundChannel4Prefab
			{
				GBATEK::NoiseControl control;
				GBATEK::NoiseFrequency frequency;
			};

			union USoundChannel
			{
				SoundChannel1Prefab channel1;
				SoundChannel2Prefab channel2;
				SoundChannel4Prefab channel4;
			};

			struct NoteEvent
			{
				u32 deltaTick;
				int channelId;
				USoundChannel channel;

				void SetChannelProperties(const SoundChannel1Prefab& properties);
				void SetChannelProperties(const SoundChannel2Prefab& properties);
				void SetChannelProperties(const SoundChannel4Prefab& properties);
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