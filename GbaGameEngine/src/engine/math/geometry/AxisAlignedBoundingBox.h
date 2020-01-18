#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"

struct AxisAlignedBoundingBox2
{
	Vector2<tFixedPoint8> min, max;

	AxisAlignedBoundingBox2(Vector2<tFixedPoint8> min, Vector2<tFixedPoint8> max)
	{
		this->min = min;
		this->max = max;
	}

	bool Intersects(const AxisAlignedBoundingBox2& other) const
	{
		return
			(min.x < other.max.x && max.x > other.min.x) &&
			(min.y < other.max.y && max.y > other.min.y);
	}

	bool IntersectsOrTouches(const AxisAlignedBoundingBox2& other) const
	{
		return
			(min.x <= other.max.x && max.x >= other.min.x) &&
			(min.y <= other.max.y && max.y >= other.min.y);
	}

	void Translate(Vector2<tFixedPoint8> translation)
	{
		min += translation;
		max += translation;
	}
};
