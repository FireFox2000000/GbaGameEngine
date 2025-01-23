#pragma once
#include "engine/base/core/stl/FixedPoint.h"

struct Circle
{
	FPi8 radius;

	Circle() = default;
	Circle(FPi8 radius);
};
