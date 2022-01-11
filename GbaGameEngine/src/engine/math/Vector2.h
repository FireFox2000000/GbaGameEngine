#pragma once
#include "engine/math/Math.h"

template <class T>
struct Vector2
{
	T x, y;

	// Constructors
	inline Vector2() : x(0), y(0) {};
	inline Vector2(T x, T y) : x(x), y(y) {};
	
	// Operator overloads
	inline bool operator == (const Vector2<T>& vec) const
	{
		return x == vec.x && y == vec.y;
	}
	inline bool operator != (const Vector2<T>& vec) const
	{ 
		return !(*this == vec); 
	}
	inline Vector2& operator += (const Vector2& vec)
	{
		x += static_cast<T>(vec.x);
		y += static_cast<T>(vec.y);
		return *this;
	}
	inline Vector2& operator -= (const Vector2& vec)
	{
		x -= static_cast<T>(vec.x);
		y -= static_cast<T>(vec.y);
		return *this;
	}
	inline Vector2& operator *= (float scaler)
	{
		x *= static_cast<T>(scaler);
		y *= static_cast<T>(scaler);
		return *this;
	}
	inline Vector2& operator /= (float scaler)
	{
		x /= static_cast<T>(scaler);
		y /= static_cast<T>(scaler);
		return *this;
	}

	inline Vector2 operator+(const Vector2& vec) const { return Vector2(*this) += vec; }
	inline Vector2 operator-(const Vector2& vec) const { return Vector2(*this) -= vec; }
	template <class Scalar> inline Vector2 operator*(Scalar scalar) const { return Vector2(*this) *= scalar; }
	template <class Scalar> inline Vector2 operator/(Scalar scalar) const { return Vector2(*this) /= scalar; }

	template<class U>
	inline operator Vector2<U>() const { return Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y)); }
	
	// Regular functions
	float Magnitude() const;
	Vector2 Normal() const;
	
	// Static functions
	T MagnitudeSqrd() const;

	static Vector2 PointToVector(const Vector2& pointA, const Vector2& pointB);

	const static Vector2 Zero;
	const static Vector2 Right;
	const static Vector2 Left;
	const static Vector2 Up;
	const static Vector2 Down;
};

template <class T> const Vector2<T> Vector2<T>::Zero = Vector2<T>(0, 0);
template <class T> const Vector2<T> Vector2<T>::Right = Vector2<T>(1, 0);
template <class T> const Vector2<T> Vector2<T>::Left = Vector2<T>::Right * -1;
template <class T> const Vector2<T> Vector2<T>::Up = Vector2<T>(0, 1);
template <class T> const Vector2<T> Vector2<T>::Down = Vector2<T>::Up * -1;

template <class T> inline Vector2<T> operator*(float scalar, const Vector2<T>& vec) { return Vector2<T>(vec) * scalar; }
template <class T> inline Vector2<T> operator/(float scalar, const Vector2<T>& vec) { return Vector2<T>(vec) / scalar; }
template <class T> inline Vector2<T> operator*(int scalar, const Vector2<T>& vec) { return Vector2<T>(vec) * (float)scalar; }
template <class T> inline Vector2<T> operator/(int scalar, const Vector2<T>& vec) { return Vector2<T>(vec) / (float)scalar; }
template <class T> inline Vector2<T> operator*(double scalar, const Vector2<T>& vec) { return Vector2<T>(vec) * scalar; }
template <class T> inline Vector2<T> operator/(double scalar, const Vector2<T>& vec) { return Vector2<T>(vec) / scalar; }

typedef Vector2<float> Vector2f;

template<> float Vector2f::Magnitude() const;
template<> Vector2f Vector2f::Normal() const;
template<> Vector2f Vector2f::PointToVector(const Vector2f& pointA, const Vector2f& pointB);

template<class T>
inline T Vector2<T>::MagnitudeSqrd() const
{
	return x * x + y * y;
}
