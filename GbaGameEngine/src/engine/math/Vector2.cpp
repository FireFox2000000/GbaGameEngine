#include "Vector2.h"
#include "engine/math/Math.h"

template<> float Vector2f::Magnitude() const
{
	return sqrt(MagnitudeSqr());
}

template<> float Vector2f::MagnitudeSqr() const
{ 
	return (x * x + y * y);
}

template<> Vector2f Vector2f::Normal() const
{
	return Vector2(-y, x);
}

template<> Vector2f Vector2f::Normalised() const
{
	float magnitude = this->Magnitude();
	return magnitude > 0 ? *this / magnitude : Vector2::Zero;	// Todo, don't actually return zero vector
}

template<> float Vector2f::Length(const Vector2& a, const Vector2& b)
{
	return sqrt(LengthSqrd(a, b));
}

template<> float Vector2f::LengthSqrd(const Vector2f& a, const Vector2f& b)
{
	return (a - b).MagnitudeSqr();
}

template<> float Vector2f::Dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

template<> Vector2f Vector2f::Projection(const Vector2f& ab, const Vector2f& ac)
{
	return (Vector2f::Dot(ab, ac) / ((ac.x * ac.x) + (ac.y * ac.y)) * ac);
}

template<> float Vector2f::Determinant(const Vector2f& u, const Vector2f& v)
{
	return u.x * v.y - u.y * v.x;
}

template<> Vector2f Vector2f::PointToVector(const Vector2f& pointA, const Vector2f& pointB)
{
	return Vector2f(pointB.x - pointA.x, pointB.y - pointA.y);
}
