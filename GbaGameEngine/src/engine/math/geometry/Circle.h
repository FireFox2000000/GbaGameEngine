#pragma once
#include "engine/base/core/stl/FixedPoint.h"

struct Circle
{
	tFixedPoint8 radius;

	Circle() = default;
	Circle(tFixedPoint8 radius);
};
