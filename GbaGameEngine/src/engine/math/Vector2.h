#pragma once

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
	inline bool operator == (const Vector2& vec)
	{
		return x == vec.x && y == vec.y;
	}
	inline bool operator != (const Vector2& vec) 
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

	inline Vector2 operator+(const Vector2& vec) { return Vector2(*this) += vec; }
	inline Vector2 operator-(const Vector2& vec) { return Vector2(*this) -= vec; }
	inline Vector2 operator*(float scalar) { return Vector2(*this) *= scalar; }
	inline Vector2 operator/(float scalar) { return Vector2(*this) /= scalar; }

	inline Vector2 operator*(int scalar) { return Vector2(*this) * (float)scalar; }
	inline Vector2 operator/(int scalar) { return Vector2(*this) / (float)scalar; }
	
	// Regular functions
	float Magnitude();
	float MagnitudeSqr();
	Vector2 Normal();
	Vector2 Normalized();
	
	// Static functions
	static float Distance(Vector2 a, Vector2 b);
	static float DistanceSqr(Vector2 a, Vector2 b);
	static float Dot(Vector2 a, Vector2 b);
	static Vector2 Projection(Vector2 ab, Vector2 ac);
	static float Determinant(Vector2 u, Vector2 v);
	static Vector2 PointToVector(Vector2 pointA, Vector2 pointB);

	inline static Vector2 Zero() { return Vector2(0, 0); }
	inline static Vector2 Right() { return Vector2(1, 0); }
	inline static Vector2 Left() { return Vector2::Right() * -1; }
	inline static Vector2 Up() { return Vector2(0, 1); }
	inline static Vector2 Down() { return Vector2::Up() * -1; }
};

inline Vector2 operator*(float scalar, const Vector2& vec) { return Vector2(vec) * scalar; }
inline Vector2 operator/(float scalar, const Vector2& vec) { return Vector2(vec) / scalar; }

inline Vector2 operator*(int scalar, const Vector2& vec) { return Vector2(vec) * (float)scalar; }
inline Vector2 operator/(int scalar, const Vector2& vec) { return Vector2(vec) / (float)scalar; }
