#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"

namespace Component
{
	struct Rigidbody
	{
		Vector2<tFixedPoint24> gravity = Vector2<tFixedPoint24>(0, -9.8f);
		Vector2<tFixedPoint24> velocity = Vector2<tFixedPoint24>::Zero;
	};
}
