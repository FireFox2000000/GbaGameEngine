#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

namespace Component
{
	namespace UI
	{
		struct ScreenTransform
		{
			Vector2<FPi16> position;
			Vector2<FPi16> scale{ 1, 1 };
		};
	}
}
