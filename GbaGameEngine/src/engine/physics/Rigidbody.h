#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"

namespace Component
{
	struct Rigidbody
	{
		Vector2<FPi24> gravity = Vector2<FPi24>(0, -9.8f);
		Vector2<FPi24> velocity = Vector2<FPi24>::Zero;
	};
}
