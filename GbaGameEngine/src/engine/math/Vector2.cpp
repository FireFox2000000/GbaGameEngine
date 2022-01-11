#include "Vector2.h"

template<> float Vector2f::Magnitude() const
{
	return sqrt(MagnitudeSqrd());
}

template<> Vector2f Vector2f::Normal() const
{
	return Vector2(-y, x);
}

template<> Vector2f Vector2f::PointToVector(const Vector2f& pointA, const Vector2f& pointB)
{
	return Vector2f(pointB.x - pointA.x, pointB.y - pointA.y);
}
