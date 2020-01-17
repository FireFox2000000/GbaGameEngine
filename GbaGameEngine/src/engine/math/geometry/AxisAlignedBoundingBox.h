#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Math.h"

struct AxisAlignedBoundingBox2
{
	Vector2<tFixedPoint8> min, max;

	AxisAlignedBoundingBox2() = default;

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

	bool IntersectsOrTounches(const AxisAlignedBoundingBox2& other) const
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

	template<typename T>
	Vector2<T> Clamp(Vector2<T> point) const
	{
		point.x = MIN(point.x, max.x);
		point.x = MAX(point.x, min.x);

		point.y = MIN(point.y, max.y);
		point.y = MAX(point.y, min.y);

		return point;
	}

	template<typename T>
	bool Contains(Vector2<T> point) const
	{
		return point.x <= max.x && point.x >= min.x && point.y <= max.y && point.y >= min.y;
	}
};
