#pragma once
#include "engine/gba/GBAAttributes.h"
#include "engine/gba/bios/GBABios.h"

namespace VisualBoyAdvance
{
	// Don't let the compiler optimise seemingly empty bios call statements away
#pragma GCC push_options
#pragma GCC optimize ("O0")

	// Will crash on actual hardware
	inline void AGBPrint(const char* message)
	{
		BiosSystemCall(0xFF);
	}
}

#pragma GCC pop_options
