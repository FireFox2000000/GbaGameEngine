#pragma once
#include <concepts>

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

template<std::integral TIntergral, u8 FRACTIONAL_BITS>
class FixedPoint
{
	TIntergral storage;

	inline TIntergral GetIntStorage() const
	{
		return storage >> FRACTIONAL_BITS;
	}

	inline TIntergral GetFloatStorage() const
	{
		return storage & BITS_U32(FRACTIONAL_BITS);
	}

	// Loses float precision easily
	inline FixedPoint<TIntergral, FRACTIONAL_BITS>& MulHalfShift(const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) {
		storage = ((int)storage >> (FRACTIONAL_BITS / 2)) * (b.storage >> (FRACTIONAL_BITS / 2));
		return *this;
	}

	// High chance of encountering overflow
	inline FixedPoint<TIntergral, FRACTIONAL_BITS>& Mul(const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) {
		storage = ((int)storage * b.storage) >> FRACTIONAL_BITS;
		return *this;
	}

public:
	inline FixedPoint() : storage(0)
	{
	}

	inline FixedPoint(const FixedPoint<TIntergral, FRACTIONAL_BITS>& that)
	{
		*this = that;
	}

	inline TIntergral GetStorage() const
	{
		return storage;
	}

	inline void SetStorage(TIntergral val) volatile
	{
		storage = val;
	}

	template<class T, u8 BITS>
	FixedPoint(const FixedPoint<T, BITS>& that)
	{
		// Todo- Handle signed and unsigned types
		int shiftDir = int(FRACTIONAL_BITS) - BITS;

		if (shiftDir > 0)
			storage = that.GetStorage() << shiftDir;
		else
			storage = that.GetStorage() >> -shiftDir;
	}


	static constexpr TIntergral FloatCompress(float val)
	{
		return TIntergral(val * (1 << FRACTIONAL_BITS) + 0.5f);
	}

	static constexpr float FloatDecompress(TIntergral val)
	{
		return (1.0f / (float)(1 << FRACTIONAL_BITS)) * ((int)val);
	}

	constexpr inline FixedPoint(int val) : storage(val << FRACTIONAL_BITS) {}
	constexpr inline FixedPoint(float val) : storage(FloatCompress(val)) {}
	constexpr inline FixedPoint(double val) : storage(TIntergral(val * (1 << FRACTIONAL_BITS) + 0.5)) {}

	inline int ToInt() const
	{
		return static_cast<int>(GetIntStorage());
	}

	inline int ToRoundedInt() const
	{
		return static_cast<int>((storage + (1 << FRACTIONAL_BITS) / 2) >> FRACTIONAL_BITS);
	}

	constexpr inline float ToFloat() const
	{
		return FloatDecompress(storage);
	}

	inline double ToDouble() const
	{
		return (1.0 / (double)(1 << FRACTIONAL_BITS)) * ((int)storage);
	}

	static constexpr inline u8 GetFpLevel() { return FRACTIONAL_BITS; }

	inline operator int() const { return ToInt(); }
	inline operator float() const { return ToFloat(); }
	inline operator double() const { return ToDouble(); }

	template<class T, u8 BITS>
	inline operator FixedPoint<T, BITS>() const { return FixedPoint<T, BITS>(*this); }

	/*******************************************************************************/

	inline FixedPoint<TIntergral, FRACTIONAL_BITS>& operator += (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b)
	{
		storage += b.storage;
		return *this;
	}
	inline FixedPoint<TIntergral, FRACTIONAL_BITS>& operator -= (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b)
	{
		storage -= b.storage;
		return *this;
	}

	inline FixedPoint<TIntergral, FRACTIONAL_BITS>& operator *= (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b)
	{
		return MulHalfShift(b);
	}

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<TIntergral, FRACTIONAL_BITS>& operator /= (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b)
	{
		storage = storage * (1 << FRACTIONAL_BITS) / b.storage;
		return *this;
	}

	inline FixedPoint<TIntergral, FRACTIONAL_BITS> operator+(const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS>(*this) += b; }
	inline FixedPoint<TIntergral, FRACTIONAL_BITS> operator-(const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS>(*this) -= b; }
	inline FixedPoint<TIntergral, FRACTIONAL_BITS> operator*(const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS>(*this) *= b; }

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<TIntergral, FRACTIONAL_BITS> operator/(const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS>(*this) /= b; }

	inline bool operator > (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const
	{
		return storage > b.storage;
	}

	inline bool operator < (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const
	{
		return storage < b.storage;
	}

	inline bool operator <= (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const
	{
		return !(*this > b);
	}

	inline bool operator >= (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const
	{
		return !(*this < b);
	}

	inline bool operator == (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const
	{
		return storage == b.storage;
	}

	inline bool operator != (const FixedPoint<TIntergral, FRACTIONAL_BITS>& b) const
	{
		return !(*this == b);
	}

	/*******************************************************************************/

	inline FixedPoint<TIntergral, FRACTIONAL_BITS> operator*(const int& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS>(*this) *= FixedPoint<TIntergral, FRACTIONAL_BITS>(b); }

	inline bool operator > (const int& b) const
	{
		return *this > FixedPoint<TIntergral, FRACTIONAL_BITS>(b);
	}

	inline bool operator < (const int& b) const
	{
		return *this < FixedPoint<TIntergral, FRACTIONAL_BITS>(b);
	}

	inline bool operator <= (const int& b) const
	{
		return *this <= FixedPoint<TIntergral, FRACTIONAL_BITS>(b);
	}

	inline bool operator >= (const int& b) const
	{
		return *this >= FixedPoint<TIntergral, FRACTIONAL_BITS>(b);
	}

	inline bool operator == (const int& b) const
	{
		return *this == FixedPoint<TIntergral, FRACTIONAL_BITS>(b);
	}

	inline bool operator != (const int& b) const
	{
		return *this != FixedPoint<TIntergral, FRACTIONAL_BITS>(b);
	}
};

using tFixedPoint8 = FixedPoint<int, 8>;
using tFixedPoint16 = FixedPoint<int, 16>;
using tFixedPoint24 = FixedPoint<int, 24>;
