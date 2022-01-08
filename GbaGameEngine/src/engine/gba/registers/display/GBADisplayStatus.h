#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

// http://www.coranac.com/tonc/text/video.htm

namespace GBA
{
	namespace DisplayStatus
	{
		bool InVBlank();
		bool InHBlank();
		vu16 VCount();
		void EnableVBlankInterrupts();
		void EnableHBlankInterrupts();
	};
}
