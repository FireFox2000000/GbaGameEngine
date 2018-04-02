#include "Vector2.h"
#include <math.h>

float Vector2::Magnitude()
{
	return sqrt(MagnitudeSqr());
}

float Vector2::MagnitudeSqr()
{ 
	return (x * x + y * y);
}

Vector2 Vector2::Normal()
{
	return Vector2(-y, x);
}

Vector2 Vector2::Normalized() 
{
	return *this / this->Magnitude();
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
	return sqrt(DistanceSqr(a, b));
}

float Vector2::DistanceSqr(Vector2 a, Vector2 b)
{
	return (a - b).MagnitudeSqr();
}

float Vector2::Dot(Vector2 a, Vector2 b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2 Vector2::Projection(Vector2 ab, Vector2 ac)
{
	return (Vector2::Dot(ab, ac) / ((ac.x * ac.x) + (ac.y * ac.y)) * ac);
}

float Vector2::Determinant(Vector2 u, Vector2 v)
{
	return u.x * v.y - u.y * v.x;
}

Vector2 Vector2::PointToVector(Vector2 pointA, Vector2 pointB)
{
	return Vector2(pointB.x - pointA.x, pointB.y - pointA.y);
}
