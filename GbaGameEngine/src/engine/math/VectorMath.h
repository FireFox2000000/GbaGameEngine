#pragma once
#include "Vector2.h"

namespace VectorMath
{
	template<class T>
	T Dot(const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	/// <summary>
	/// Projection of Vector A on a pre-normalised Vector B. Optimised version if we already know that b is a normal vector
	/// </summary>
	/// <param name="a">Comparision vector</param>
	/// <param name="normal">A normalised vector. No safety checking, we're trusting this is actually mag 1.</param>
	/// <returns>Returns the projection of a on b</returns>
	template<class T>
	Vector2<T> ProjectionNormalised(const Vector2<T>& a, const Vector2<T>& normal)
	{
		auto dot = VectorMath::Dot(a, normal);
		return Vector2<T>(dot * normal.x, dot * normal.y);
	}

	template<class T>
	Vector2<T> Projection(const Vector2<T>& ab, const Vector2<T>& ac)
	{
		T dot = Dot(ab, ac);
		T magSqrd = ac.MagnitudeSqrd();

		return Vector2<T>(dot * ac.x / magSqrd, dot * ac.y / magSqrd);
	}

	template<class T>
	inline T LengthSqrd(const Vector2<T>& a, const Vector2<T>& b)
	{
		return (a - b).MagnitudeSqrd();
	}

	/// <summary>
	/// Calculate a vector of magnitude 1 in the same direction as the input vector.
	/// </summary>
	inline Vector2f Normalised(const Vector2f& a)
	{
		return a * Math::InvSqrt(a.MagnitudeSqrd());
	}

	template<class T>
	inline T Determinant(const Vector2<T>& u, const Vector2<T>& v)
	{
		return u.x * v.y - u.y * v.x;
	}
}
