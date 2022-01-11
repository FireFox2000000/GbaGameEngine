#include "Math.h"
#include "engine/base/core/stl/FixedPoint.h"

#define SIN_LUT_SIZE 512
#define M_PI 3.1415926535f

// We calculate the sin LUT during compile time. Avoids having to link in a seperate file while still avoiding startup performance tank if this was done at run-time. 
struct SinLookupTable {
	s16 arr[SIN_LUT_SIZE];	// Stored as FixedPoint<u16, FRACTIONAL_BITS>, need primitive for constexpr

	constexpr SinLookupTable() : arr()
	{
		for (int i = 0; i < SIN_LUT_SIZE; ++i)
		{
			arr[i] = FixedPoint<s16, SIN_LUT_FRACTIONAL_BITS>::FloatCompress(sin(i * 2.f * M_PI / SIN_LUT_SIZE));
		}
	}
};

constexpr SinLookupTable sinLut;

int Math::Mod(int x, unsigned int N)
{
	return (x % N + N) % N;
}

s32 Math::Sin(u16 theta)
{
	s16 val = sinLut.arr[(theta >> 7) & 0x1FF];	// Divide by 512 and mask off the rest

	return val;// FixedPoint<s16, FRACTIONAL_BITS>::FloatDecompress(val);
}

s32 Math::Cos(u16 theta)
{
	s16 val = sinLut.arr[((theta >> 7) + 128) & 0x1FF];
	return val;// FixedPoint<s16, FRACTIONAL_BITS>::FloatDecompress(val);
}

// Quake fast inv sqrt - https://betterexplained.com/articles/understanding-quakes-fast-inverse-square-root/
float Math::InvSqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;            // store floating-point bits in integer
	i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
	x = *(float*)&i;              // convert new bits into float
	x = x * (1.5f - xhalf * x * x);     // One round of Newton's method
	return x;
}
