#pragma once
#include "engine/base/core/stl/FixedPoint.h"

struct Circle
{
	tFixedPoint8 radius;

	Circle() = default;

	inline Circle(tFixedPoint8 radius)
	{
		this->radius = radius;
	}
};
