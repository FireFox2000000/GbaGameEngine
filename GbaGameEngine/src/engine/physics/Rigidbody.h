#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"

namespace Component
{
	struct Rigidbody
	{
		Vector2<FPi16> gravity = Vector2<FPi16>(0, -9.8f);
		Vector2<FPi16> velocity = Vector2<FPi16>::Zero;
	};
}
