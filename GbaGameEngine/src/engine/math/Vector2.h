#pragma once
#include "engine/math/Math.h"

template <typename T>
struct Vector2
{
	T x = 0, y = 0;

	// Operator overloads
	constexpr inline bool operator == (const Vector2<T>& vec) const
	{
		return x == vec.x && y == vec.y;
	}
	constexpr inline bool operator != (const Vector2<T>& vec) const
	{ 
		return !(*this == vec); 
	}
	constexpr inline Vector2& operator += (const Vector2& vec)
	{
		x += static_cast<T>(vec.x);
		y += static_cast<T>(vec.y);
		return *this;
	}
	constexpr inline Vector2& operator -= (const Vector2& vec)
	{
		x -= static_cast<T>(vec.x);
		y -= static_cast<T>(vec.y);
		return *this;
	}

	template<typename TScalar>
	constexpr inline Vector2& operator *= (TScalar scaler)
	{
		x *= static_cast<T>(scaler);
		y *= static_cast<T>(scaler);
		return *this;
	}

	template<typename TScalar>
	constexpr inline Vector2& operator /= (TScalar scaler)
	{
		x /= static_cast<T>(scaler);
		y /= static_cast<T>(scaler);
		return *this;
	}

	constexpr inline Vector2 operator+(const Vector2& vec) const { return Vector2(*this) += vec; }
	constexpr inline Vector2 operator-(const Vector2& vec) const { return Vector2(*this) -= vec; }

	template <class Scalar> 
	constexpr inline Vector2 operator*(Scalar scalar) const { return Vector2(*this) *= scalar; }

	template <class Scalar> 
	constexpr inline Vector2 operator/(Scalar scalar) const { return Vector2(*this) /= scalar; }

	template<class U>
	explicit inline operator Vector2<U>() const { return Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y)); }
	
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

template <class T> constexpr Vector2<T> Vector2<T>::Zero = Vector2<T>(0, 0);
template <class T> constexpr Vector2<T> Vector2<T>::Right = Vector2<T>(1, 0);
template <class T> constexpr Vector2<T> Vector2<T>::Left = Vector2<T>::Right * -1;
template <class T> constexpr Vector2<T> Vector2<T>::Up = Vector2<T>(0, 1);
template <class T> constexpr Vector2<T> Vector2<T>::Down = Vector2<T>::Up * -1;

template <class T, typename TScalar> 
constexpr inline Vector2<T> operator*(TScalar scalar, const Vector2<T>& vec) { return Vector2<T>(vec) * scalar; }

template <class T, typename TScalar> 
constexpr inline Vector2<T> operator/(TScalar scalar, const Vector2<T>& vec) { return Vector2<T>(vec) / scalar; }

typedef Vector2<float> Vector2f;

template<> float Vector2f::Magnitude() const;
template<> Vector2f Vector2f::Normal() const;
template<> Vector2f Vector2f::PointToVector(const Vector2f& pointA, const Vector2f& pointB);

template<class T>
inline T Vector2<T>::MagnitudeSqrd() const
{
	return x * x + y * y;
}
