#pragma once
#include <type_traits>	// for is_integral

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

// If you don't provide an integer type as the IntType parameter then I hate you.
template<class IntType, u8 FRACTIONAL_BITS>
class FixedPoint
{
	IntType storage;

	inline IntType GetIntStorage() const
	{
		return storage >> FRACTIONAL_BITS;
	}

	inline IntType GetFloatStorage() const
	{
		return storage & BITS_U32(FRACTIONAL_BITS);
	}

	// Looses float precision easily
	inline FixedPoint<IntType, FRACTIONAL_BITS>& MulHalfShift(const FixedPoint<IntType, FRACTIONAL_BITS>& b) {
		storage = ((int)storage >> (FRACTIONAL_BITS / 2)) * (b.storage >> (FRACTIONAL_BITS / 2));
		return *this;
	}

	// High chance of encountering overflow
	inline FixedPoint<IntType, FRACTIONAL_BITS>& Mul(const FixedPoint<IntType, FRACTIONAL_BITS>& b) {
		storage = ((int)storage * b.storage) >> FRACTIONAL_BITS;
		return *this;
	}

public:
	inline FixedPoint() : storage(0)
	{
		STATIC_ASSERT(std::is_integral<IntType>::value, "Integral required.");
	}

	inline FixedPoint(const FixedPoint<IntType, FRACTIONAL_BITS>& that)
	{
		*this = that;
	}

	inline IntType GetStorage() const
	{
		return storage;
	}

	inline void SetStorage(IntType val) volatile
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


	static constexpr IntType FloatCompress(float val)
	{
		return IntType(val * (1 << FRACTIONAL_BITS) + 0.5f);
	}

	static constexpr float FloatDecompress(IntType val)
	{
		return (1.0f / (float)(1 << FRACTIONAL_BITS)) * ((int)val);
	}

	constexpr inline FixedPoint(int val) : storage(val << FRACTIONAL_BITS) {}
	constexpr inline FixedPoint(float val) : storage(FloatCompress(val)) {}
	constexpr inline FixedPoint(double val) : storage(IntType(val * (1 << FRACTIONAL_BITS) + 0.5)) {}

	inline int ToInt() const
	{
		return (int)(GetIntStorage());
	}

	inline int ToRoundedInt() const
	{
		return (int)((storage + (1 << FRACTIONAL_BITS) / 2) >> FRACTIONAL_BITS);
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

	inline FixedPoint<IntType, FRACTIONAL_BITS>& operator += (const FixedPoint<IntType, FRACTIONAL_BITS>& b)
	{
		storage += b.storage;
		return *this;
	}
	inline FixedPoint<IntType, FRACTIONAL_BITS>& operator -= (const FixedPoint<IntType, FRACTIONAL_BITS>& b)
	{
		storage -= b.storage;
		return *this;
	}

	inline FixedPoint<IntType, FRACTIONAL_BITS>& operator *= (const FixedPoint<IntType, FRACTIONAL_BITS>& b)
	{
		return MulHalfShift(b);
	}

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<IntType, FRACTIONAL_BITS>& operator /= (const FixedPoint<IntType, FRACTIONAL_BITS>& b)
	{
		storage = storage * (1 << FRACTIONAL_BITS) / b.storage;
		return *this;
	}

	inline FixedPoint<IntType, FRACTIONAL_BITS> operator+(const FixedPoint<IntType, FRACTIONAL_BITS>& b) const { return FixedPoint<IntType, FRACTIONAL_BITS>(*this) += b; }
	inline FixedPoint<IntType, FRACTIONAL_BITS> operator-(const FixedPoint<IntType, FRACTIONAL_BITS>& b) const { return FixedPoint<IntType, FRACTIONAL_BITS>(*this) -= b; }
	inline FixedPoint<IntType, FRACTIONAL_BITS> operator*(const FixedPoint<IntType, FRACTIONAL_BITS>& b) const { return FixedPoint<IntType, FRACTIONAL_BITS>(*this) *= b; }

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<IntType, FRACTIONAL_BITS> operator/(const FixedPoint<IntType, FRACTIONAL_BITS>& b) const { return FixedPoint<IntType, FRACTIONAL_BITS>(*this) /= b; }

	inline bool operator > (const FixedPoint<IntType, FRACTIONAL_BITS>& b) const
	{
		return storage > b.storage;
	}

	inline bool operator < (const FixedPoint<IntType, FRACTIONAL_BITS>& b) const
	{
		return storage < b.storage;
	}

	inline bool operator <= (const FixedPoint<IntType, FRACTIONAL_BITS>& b) const
	{
		return !(*this > b);
	}

	inline bool operator >= (const FixedPoint<IntType, FRACTIONAL_BITS>& b) const
	{
		return !(*this < b);
	}

	inline bool operator == (const FixedPoint<IntType, FRACTIONAL_BITS>& b) const
	{
		return storage == b.storage;
	}

	inline bool operator != (const FixedPoint<IntType, FRACTIONAL_BITS>& b) const
	{
		return !(*this == b);
	}

	/*******************************************************************************/

	inline FixedPoint<IntType, FRACTIONAL_BITS> operator*(const int& b) const { return FixedPoint<IntType, FRACTIONAL_BITS>(*this) *= FixedPoint<IntType, FRACTIONAL_BITS>(b); }

	inline bool operator > (const int& b) const
	{
		return *this > FixedPoint<IntType, FRACTIONAL_BITS>(b);
	}

	inline bool operator < (const int& b) const
	{
		return *this < FixedPoint<IntType, FRACTIONAL_BITS>(b);
	}

	inline bool operator <= (const int& b) const
	{
		return *this <= FixedPoint<IntType, FRACTIONAL_BITS>(b);
	}

	inline bool operator >= (const int& b) const
	{
		return *this >= FixedPoint<IntType, FRACTIONAL_BITS>(b);
	}

	inline bool operator == (const int& b) const
	{
		return *this == FixedPoint<IntType, FRACTIONAL_BITS>(b);
	}

	inline bool operator != (const int& b) const
	{
		return *this != FixedPoint<IntType, FRACTIONAL_BITS>(b);
	}
};

using tFixedPoint8 = FixedPoint<int, 8>;
using tFixedPoint16 = FixedPoint<int, 16>;
using tFixedPoint24 = FixedPoint<int, 24>;
