#ifndef PRAGMA_ONCE_ENGINE_MATH_VECTOR2_H
#define PRAGMA_ONCE_ENGINE_MATH_VECTOR2_H

template <class T>
struct Vector2
{
	T x, y;

	// Constructors
	inline Vector2() : x(0), y(0) {};
	inline Vector2(int x, int y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {};
	inline Vector2(float x, float y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {};
	inline Vector2(double x, double y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {};
	inline Vector2(const Vector2& that) { *this = that; }
	
	// Operator overloads
	inline bool operator == (const Vector2& vec) const
	{
		return x == vec.x && y == vec.y;
	}
	inline bool operator != (const Vector2& vec) const
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
	inline Vector2 operator*(float scalar) const { return Vector2(*this) *= scalar; }
	inline Vector2 operator/(float scalar) const { return Vector2(*this) /= scalar; }

	inline Vector2 operator*(int scalar) const { return Vector2(*this) * (float)scalar; }
	inline Vector2 operator/(int scalar) const { return Vector2(*this) / (float)scalar; }
	inline Vector2 operator*(double scalar) const { return Vector2(*this) *= scalar; }
	inline Vector2 operator/(double scalar) const { return Vector2(*this) /= scalar; }

	template<class U>
	inline operator Vector2<U>() const { return Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y)); }
	
	// Regular functions
	float Magnitude() const;
	float MagnitudeSqr() const;
	Vector2 Normal() const;
	Vector2 Normalised() const;
	
	// Static functions
	static float Length(const Vector2& a, const Vector2& b);
	static float LengthSqrd(const Vector2& a, const Vector2& b);
	static float Dot(const Vector2& a, const Vector2& b);
	static Vector2 Projection(const Vector2& ab, const Vector2& ac);
	static float Determinant(const Vector2& u, const Vector2& v);
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
template<> float Vector2f::MagnitudeSqr() const;
template<> Vector2f Vector2f::Normal() const;
template<> Vector2f Vector2f::Normalised() const;

template<> float Vector2f::Length(const Vector2f& a, const Vector2f& b);
template<> float Vector2f::LengthSqrd(const Vector2f& a, const Vector2f& b);
template<> float Vector2f::Dot(const Vector2f& a, const Vector2f& b);
template<> Vector2f Vector2f::Projection(const Vector2f& ab, const Vector2f& ac);
template<> float Vector2f::Determinant(const Vector2f& u, const Vector2f& v);
template<> Vector2f Vector2f::PointToVector(const Vector2f& pointA, const Vector2f& pointB);

#endif
