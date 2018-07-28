#ifndef PRAGMA_ONCE_ENGINE_MATH_VECTOR2_H
#define PRAGMA_ONCE_ENGINE_MATH_VECTOR2_H

struct Vector2
{
	float x, y;

	// Constructors
	inline Vector2() : x(0), y(0) {};
	inline Vector2(int x, int y) : x(x), y(y) {};
	inline Vector2(float x, float y) : x(x), y(y) {};
	inline Vector2(double x, double y) : x((float)x), y((float)y) {};
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
		x += vec.x;
		y += vec.y;
		return *this;
	}
	inline Vector2& operator -= (const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	inline Vector2& operator *= (float scaler)
	{
		x *= scaler;
		y *= scaler;
		return *this;
	}
	inline Vector2& operator /= (float scaler)
	{
		x /= scaler;
		y /= scaler;
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

inline Vector2 operator*(float scalar, const Vector2& vec) { return Vector2(vec) * scalar; }
inline Vector2 operator/(float scalar, const Vector2& vec) { return Vector2(vec) / scalar; }

inline Vector2 operator*(int scalar, const Vector2& vec) { return Vector2(vec) * (float)scalar; }
inline Vector2 operator/(int scalar, const Vector2& vec) { return Vector2(vec) / (float)scalar; }

inline Vector2 operator*(double scalar, const Vector2& vec) { return Vector2(vec) * scalar; }
inline Vector2 operator/(double scalar, const Vector2& vec) { return Vector2(vec) / scalar; }

#endif
