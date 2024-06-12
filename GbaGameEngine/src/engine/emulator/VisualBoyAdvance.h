#pragma once
#include "gbatek/Bios.h"
#include "gbatek/internal/Internal.h"

namespace VisualBoyAdvance
{
	// Don't let the compiler optimise seemingly empty bios call statements away
GBA_OPTIONS_PUSH_OPTIMIZE_O0

	// Will crash on actual hardware
	inline void AGBPrint(const char* message)
	{
		GBA_BiosSystemCall(0xFF);
	}

GBA_OPTIONS_POP
}
