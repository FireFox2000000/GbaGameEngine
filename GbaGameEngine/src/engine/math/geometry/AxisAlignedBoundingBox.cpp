#include "AxisAlignedBoundingBox.h"

AxisAlignedBoundingBox2::AxisAlignedBoundingBox2(Vector2<FPi16> min, Vector2<FPi16> max)
{
	this->min = min;
	this->max = max;
}

bool AxisAlignedBoundingBox2::Intersects(const AxisAlignedBoundingBox2& other) const
{
	return
		(min.x < other.max.x && max.x > other.min.x) &&
		(min.y < other.max.y && max.y > other.min.y);
}

bool AxisAlignedBoundingBox2::IntersectsOrTouches(const AxisAlignedBoundingBox2& other) const
{
	return
		(min.x <= other.max.x && max.x >= other.min.x) &&
		(min.y <= other.max.y && max.y >= other.min.y);
}

void AxisAlignedBoundingBox2::Translate(Vector2<FPi16> translation)
{
	min += translation;
	max += translation;
}
