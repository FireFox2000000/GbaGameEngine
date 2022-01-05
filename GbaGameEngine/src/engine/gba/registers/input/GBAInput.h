#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

namespace GBA
{
	namespace Buttons
	{
		enum Enum
		{
			A = BIT(0),
			B = BIT(1),
			Select = BIT(2),
			Start = BIT(3),
			Right = BIT(4),
			Left = BIT(5),
			Up = BIT(6),
			Down = BIT(7),
			R = BIT(8),
			L = BIT(9),

			Count = 10
		};
	}

	class Input
	{
	public:
		static bool GetKey(Buttons::Enum key, const u32 inputChannel);

		// Note that bitmask is reversed. If the bit is set, the key is released.
		static vu32 GetInputKeys();
	};
}
