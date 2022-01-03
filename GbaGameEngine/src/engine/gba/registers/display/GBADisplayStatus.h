#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

// http://www.coranac.com/tonc/text/video.htm

namespace GBA
{
	class DisplayStatus
	{
		static bool IsStatusBitSet(int bitIndex);

	public:
		static bool InVBlank();
		static bool InHBlank();
		static vu16 VCount();
		static void EnableVBlankInterrupts();
		static void EnableHBlankInterrupts();
	};
}
