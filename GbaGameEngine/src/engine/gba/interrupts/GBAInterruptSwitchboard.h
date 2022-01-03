#pragma once

#include "GBAInterrupts.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/base/core/stl/Bitmask.h"
#include "engine/gba/GBAAttributes.h"

#include <functional>

namespace GBA
{
	namespace InterruptSwitchboard
	{
		using InterruptHandler = std::function<void()>;

		void Init();
		void RegisterHandler(Interrupts::InterruptType interruptType, InterruptHandler handler);
	};
}
