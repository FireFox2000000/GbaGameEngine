#pragma once
#include "../../base/Typedefs.h"
#include "../../base/Macros.h"

// http://www.coranac.com/tonc/text/video.htm

namespace GBA
{
	class DisplayStatus
	{
		static bool IsStatusBitSet(int bitIndex);
	public:
		inline static bool InVBlank() { return IsStatusBitSet(BIT(0)); }
		inline static bool InHBlank() { return IsStatusBitSet(BIT(1)); }
		static u16 VCount();
	};
}

