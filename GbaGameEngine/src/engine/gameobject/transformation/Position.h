#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

using tFixedPoint8 = FixedPoint<int, 8>;

namespace Component
{
	struct Position : Vector2<tFixedPoint8>
	{
	};
}
