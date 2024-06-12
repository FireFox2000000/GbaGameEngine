#pragma once
#include "gbatek/Bios.h"
#include "gbatek/internal/Internal.h"

namespace VisualBoyAdvance
{
	// Don't let the compiler optimise seemingly empty bios call statements away
GBATEK_OPTIONS_PUSH_OPTIMIZE_O0

	// Will crash on actual hardware
	inline void AGBPrint(const char* message)
	{
		GBATEK_BiosSystemCall(0xFF);
	}

GBATEK_OPTIONS_POP
}
