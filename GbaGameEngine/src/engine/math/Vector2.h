#pragma once
#include "engine/math/Math.h"
#include <concepts>

template <typename T>
struct Vector2
{
	T x = 0, y = 0;

	constexpr Vector2() = default;
	constexpr Vector2(T x, T y) : x(x), y(y) {}
	template<class U>
	explicit constexpr Vector2(const Vector2<U> other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

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

	static constexpr Vector2<T> Zero = Vector2<T>(0, 0);
	static constexpr Vector2<T> Right = Vector2<T>(1, 0);
	static constexpr Vector2<T> Left = Vector2<T>::Right * -1;
	static constexpr Vector2<T> Up = Vector2<T>(0, 1);
	static constexpr Vector2<T> Down = Vector2<T>::Up * -1;

	inline T Magnitude() const requires std::floating_point<T> { return std::sqrt(MagnitudeSqrd()); }
	inline Vector2 Normal() const { return Vector2(-y, x); }
	inline T MagnitudeSqrd() const { return x * x + y * y; }
};


template <class T, typename TScalar> 
constexpr inline Vector2<T> operator*(TScalar scalar, const Vector2<T>& vec) { return Vector2<T>(vec) * scalar; }

template <class T, typename TScalar> 
constexpr inline Vector2<T> operator/(TScalar scalar, const Vector2<T>& vec) { return Vector2<T>(vec) / scalar; }

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
