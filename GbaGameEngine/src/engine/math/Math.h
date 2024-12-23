#pragma once

#include <cmath>
#include <bit>
#include "engine/base/Typedefs.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ROUND(x) (static_cast<int>((x) + 0.5f))

#define SECONDS_TO_MILLISECONDS(x) ((x) * 1000)
#define SECONDS_TO_MICROSECONDS(x) ((x) * 1000000)
#define MILLISECONDS_TO_SECONDS(x) ((x) / 1000)

#define BYTES_TO_KB(x) ((x) / 1024.0f)
#define SIN_LUT_FRACTIONAL_BITS 12

namespace Math
{
	template<typename T>
	inline bool SignCompare(T a, T b) {
		return a * b > 0;
	}

	template<typename T, typename U, typename V>
	inline T Clamp(T v, U min, V max) {
		return MAX(MIN(v, max), min);
	}

	// Modulous operation, can deal with negative modulo operations unlike %
	int Mod(int x, unsigned int N);

	// N must be a power of 2, otherwise this will be incorrect
	inline int Power2Mod(int x, int N) { return x & (N - 1); }

	// theta's range is [0, 0xFFFF] for [0, 2 Pi]
	s32 Sin(u16 theta);

	// theta's range is [0, 0xFFFF] for [0, 2 Pi]
	s32 Cos(u16 theta);

	constexpr float InvSqrt(float x)
	{
		// Quake fast inv sqrt - https://betterexplained.com/articles/understanding-quakes-fast-inverse-square-root/
		float xhalf = 0.5f * x;
		int i = std::bit_cast<int>(x);            // store floating-point bits in integer
		i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
		x = std::bit_cast<float>(i);              // convert new bits into float
		x = x * (1.5f - xhalf * x * x);     // One round of Newton's method
		return x;
	}

	template <typename T> 
	constexpr int Sign(T val) 
	{
		return (T(0) < val) - (val < T(0));
	}

	template <typename T>
	constexpr T Abs(T val) 
	{
		return val > T(0) ? val : val * T(-1);
	}
}
