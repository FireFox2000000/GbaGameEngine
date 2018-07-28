#include "Vector2.h"
#include "engine/math/Math.h"

const Vector2 Vector2::Zero = Vector2(0, 0);
const Vector2 Vector2::Right = Vector2(1, 0);
const Vector2 Vector2::Left = Vector2::Right * -1;
const Vector2 Vector2::Up = Vector2(0, 1);
const Vector2 Vector2::Down = Vector2::Up * -1;

float Vector2::Magnitude() const
{
	return sqrt(MagnitudeSqr());
}

float Vector2::MagnitudeSqr() const
{ 
	return (x * x + y * y);
}

Vector2 Vector2::Normal() const
{
	return Vector2(-y, x);
}

Vector2 Vector2::Normalised() const
{
	float magnitude = this->Magnitude();
	return magnitude > 0 ? *this / magnitude : Vector2::Zero;	// Todo, don't actually return zero vector
}

float Vector2::Length(const Vector2& a, const Vector2& b)
{
	return sqrt(LengthSqrd(a, b));
}

float Vector2::LengthSqrd(const Vector2& a, const Vector2& b)
{
	return (a - b).MagnitudeSqr();
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2 Vector2::Projection(const Vector2& ab, const Vector2& ac)
{
	return (Vector2::Dot(ab, ac) / ((ac.x * ac.x) + (ac.y * ac.y)) * ac);
}

float Vector2::Determinant(const Vector2& u, const Vector2& v)
{
	return u.x * v.y - u.y * v.x;
}

Vector2 Vector2::PointToVector(const Vector2& pointA, const Vector2& pointB)
{
	return Vector2(pointB.x - pointA.x, pointB.y - pointA.y);
}
