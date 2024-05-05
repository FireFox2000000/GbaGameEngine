#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"

// Hertz == Cycles per seconds

namespace GBA
{
	// Configureable based on use-case
	enum TimerId : u8
	{
		Sound0,
		Sound1,
		SystemClock1,
		SystemClock2,
	};
}