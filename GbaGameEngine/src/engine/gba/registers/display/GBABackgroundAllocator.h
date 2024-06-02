#pragma once
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/Bitmask.h"

namespace GBA
{
	// Provides access to the physical background controls registers of the GBA.
	class BackgroundAllocator
	{
		static Bitmask<u8> s_backgroundPoolTracker;

	public:
		static constexpr int INVALID_BACKGROUND = -1;

		static int ReserveBackground();
		static void FreeBackground(int id);
	};
}
