#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

namespace Component
{
	namespace UI
	{
		struct ScreenTransform
		{
			Vector2<tFixedPoint8> position;
			Vector2<tFixedPoint8> scale{ .x = 1, .y = 1 };
		};
	}
}
