#pragma once
#include "IInputMap.h"
#include "engine/base/Typedefs.h"

namespace Input
{
	struct GbaKeypadMap : IInputMap
	{
		u32 keysMask = 0;

		GbaKeypadMap(u32 inputMask) : keysMask(inputMask) {}
	};
}
