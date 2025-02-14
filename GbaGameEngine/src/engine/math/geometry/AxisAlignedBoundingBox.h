#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Math.h"

struct AxisAlignedBoundingBox2
{
	Vector2<FPi16> min, max;

	AxisAlignedBoundingBox2() = default;
	AxisAlignedBoundingBox2(Vector2<FPi16> min, Vector2<FPi16> max);

	bool Intersects(const AxisAlignedBoundingBox2& other) const;
	bool IntersectsOrTouches(const AxisAlignedBoundingBox2& other) const;

	void Translate(Vector2<FPi16> translation);

	template<typename T>
	inline Vector2<T> Clamp(Vector2<T> point) const
	{
		point.x = Math::Min(point.x, max.x);
		point.x = Math::Max(point.x, min.x);

		point.y = Math::Min(point.y, max.y);
		point.y = Math::Max(point.y, min.y);

		return point;
	}

	template<typename T>
	bool Contains(Vector2<T> point) const
	{
		return point.x <= max.x && point.x >= min.x && point.y <= max.y && point.y >= min.y;
	}
};
