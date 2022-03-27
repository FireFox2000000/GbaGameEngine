#pragma once

namespace GBA
{
	namespace DMG
	{
		namespace Midi
		{
			struct NoteEvent;
		}
	}
}
namespace Midi
{
	namespace FallOfFall
	{
		const GBA::DMG::Midi::NoteEvent* begin();
		const GBA::DMG::Midi::NoteEvent* end();
	}
}
