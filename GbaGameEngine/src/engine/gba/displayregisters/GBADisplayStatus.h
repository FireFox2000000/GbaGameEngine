#pragma once
#include "../../base/Macros.h"

namespace GBA
{
	class DisplayStatus
	{
		static bool IsBitSet(int bitIndex);
	public:
		inline static bool GetVBlankStatus() { return IsBitSet(BIT(0)); }
		inline static bool GetHBlankStatus() { return IsBitSet(BIT(1)); }
	};
}

