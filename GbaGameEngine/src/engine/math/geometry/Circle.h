#pragma once
#include "engine/base/core/stl/FixedPoint.h"

struct Circle
{
	FPi16 radius;

	Circle() = default;
	Circle(FPi16 radius);
};
