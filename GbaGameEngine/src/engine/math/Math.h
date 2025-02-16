#pragma once

#include <cmath>
#include <bit>
#include "engine/base/Typedefs.h"

#define ROUND(x) (static_cast<int>((x) + 0.5f))

#define BYTES_TO_KB(x) ((x) / 1024.0f)
#define SIN_LUT_FRACTIONAL_BITS 12

namespace Math
{
	template <typename T>
	concept Arithmetic = std::is_arithmetic_v<T> || requires(T a)
	{
		a += a;
		a -= a;
		a *= a;
		a /= a;
		a == a;
		a != a;
		a > a;
		a < a;
		a >= a;
		a <= a;

		// Require all basic operations to also be compatible with int so that these operations
		// can be performed on hardcoded numbers, e.g. SecondsToMilliseconds
		a = static_cast<int>(0);
	};

	template<Arithmetic T>
	constexpr inline T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template<Arithmetic T>
	constexpr inline T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template<Arithmetic T>
	constexpr inline bool SignCompare(T a, T b)
	{
		return a * b > 0;
	}

	template<Arithmetic T, Arithmetic U, Arithmetic V>
	constexpr inline T Clamp(T v, U min, V max)
	{
		return Max(Min(v, max), min);
	}

	// Modulous operation, can deal with negative modulo operations unlike %
	constexpr inline int Mod(int x, unsigned int N) 
	{ 
		return (x % N + N) % N; 
	}

	// N must be a power of 2, otherwise this will be incorrect
	constexpr inline int Power2Mod(int x, int N)
	{ 
		return x & (N - 1); 
	}

	// theta's range is [0, 0xFFFF] for [0, 2 Pi]
	s32 Sin(u16 theta);

	// theta's range is [0, 0xFFFF] for [0, 2 Pi]
	s32 Cos(u16 theta);

	constexpr float InvSqrt(float x)
	{
		// Quake fast inv sqrt - https://betterexplained.com/articles/understanding-quakes-fast-inverse-square-root/
		// For platforms that do not have an FPU
		float xhalf = 0.5f * x;
		int i = std::bit_cast<int>(x);            // store floating-point bits in integer
		i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
		x = std::bit_cast<float>(i);              // convert new bits into float
		x = x * (1.5f - xhalf * x * x);     // One round of Newton's method
		return x;
	}

	template <Arithmetic T>
	constexpr int Sign(T val) 
	{
		return (T(0) < val) - (val < T(0));
	}

	template <Arithmetic T>
	constexpr T Abs(T val) 
	{
		return val > T(0) ? val : val * T(-1);
	}

	constexpr bool IsOdd(int val)
	{
		return (val & 1) != 0;
	}

	template<Arithmetic T>
	constexpr inline T SecondsToMilliseconds(T numeric)
	{
		return numeric * 1000;
	}

	template<Arithmetic T>
	constexpr inline T SecondsToMicroSeconds(T numeric)
	{
		return numeric * 1000000;
	}

	template<Arithmetic T>
	constexpr inline T MillisecondsToSeconds(T numeric)
	{
		return numeric / 1000;
	}
}
