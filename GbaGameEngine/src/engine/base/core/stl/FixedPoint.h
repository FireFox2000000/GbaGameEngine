#pragma once
#include <concepts>

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

/// <summary>
/// Fixed point number type. Uses integral types to fake floating point calculations by taking part 
/// of the bits and repurposing them to represent the decimal value
/// </summary>
/// <param name="TIntergral">The underlying base type where integral and decimal numbers are stored</param>
/// <param name="FRACTIONAL_BITS">The amount of bits used to store the decimal component</param>
/// <param name="TIntermediate ">The type used during various calculations such as division where overflow and
/// underflow may be prominant</param>
template<std::integral TIntergral, u8 FRACTIONAL_BITS, std::integral TIntermediate>
class FixedPoint
{
	TIntergral storage = 0;

	inline TIntergral GetIntStorage() const
	{
		return storage >> FRACTIONAL_BITS;
	}

	inline TIntergral GetFloatStorage() const
	{
		return storage & BITS_U32(FRACTIONAL_BITS);
	}

	// Loses float precision easily
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& MulHalfShift(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) {
		storage = (static_cast<int>(storage) >> (FRACTIONAL_BITS / 2)) * (b.storage >> (FRACTIONAL_BITS / 2));
		return *this;
	}

	// High chance of encountering overflow
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& Mul(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) {
		storage = (static_cast<int>(storage) * b.storage) >> FRACTIONAL_BITS;
		return *this;
	}

public:
	inline FixedPoint() : storage(0)
	{
	}

	inline FixedPoint(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& that)
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

	template<class T, u8 BITS, class V>
	FixedPoint(const FixedPoint<T, BITS, V>& that)
	{
		// Todo- Handle signed and unsigned types
		int shiftDir = static_cast<int>(FRACTIONAL_BITS) - BITS;

		if (shiftDir > 0)
			storage = static_cast<TIntergral>(that.GetStorage() << shiftDir);
		else
			storage = static_cast<TIntergral>(that.GetStorage() >> -shiftDir);
	}


	static constexpr TIntergral FloatCompress(float val)
	{
		return TIntergral(val * (1 << FRACTIONAL_BITS) + 0.5f);
	}

	static constexpr float FloatDecompress(TIntergral val)
	{
		return (1.0f / static_cast<float>(static_cast<TIntergral>(1) << FRACTIONAL_BITS)) * static_cast<int>(val);
	}

	constexpr inline FixedPoint(int val) : storage(static_cast<TIntergral>(val) << FRACTIONAL_BITS) {}
	constexpr inline FixedPoint(float val) : storage(FloatCompress(val)) {}
	constexpr inline FixedPoint(double val) : storage(static_cast<TIntergral>(val * (1 << FRACTIONAL_BITS) + 0.5)) {}

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
		return (1.0 / static_cast<double>(1 << FRACTIONAL_BITS)) * (static_cast<int>(storage));
	}

	static constexpr inline u8 GetFpLevel() { return FRACTIONAL_BITS; }

	inline operator int() const { return ToInt(); }
	inline operator float() const { return ToFloat(); }
	inline operator double() const { return ToDouble(); }

	template<class T, u8 BITS, class V>
	inline operator FixedPoint<T, BITS, V>() const { return FixedPoint<T, BITS, V>(*this); }

	/*******************************************************************************/

	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& operator += (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b)
	{
		storage += b.storage;
		return *this;
	}
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& operator -= (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b)
	{
		storage -= b.storage;
		return *this;
	}

	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& operator *= (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b)
	{
		return MulHalfShift(b);
	}

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& operator /= (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b)
	{
		// Intermediate type
		// 64 bit operations on 32 bit hardware are faster than floating point operations 
		// but still slower than 32 bit operations
		TIntermediate fpOne = static_cast<TIntermediate>(1) << FRACTIONAL_BITS;
		TIntermediate mul = storage * fpOne;

		DEBUG_ASSERTMSG(mul != 0 && mul / storage == fpOne, "Overflow detected during fixed point division");

		storage = static_cast<TIntergral>(mul / b.storage);
		return *this;
	}

	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator+(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) += b; }
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator-(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) -= b; }
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator*(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) *= b; }

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator/(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) /= b; }

	inline bool operator > (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const
	{
		return storage > b.storage;
	}

	inline bool operator < (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const
	{
		return storage < b.storage;
	}

	inline bool operator <= (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const
	{
		return !(*this > b);
	}

	inline bool operator >= (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const
	{
		return !(*this < b);
	}

	inline bool operator == (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const
	{
		return storage == b.storage;
	}

	inline bool operator != (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const
	{
		return !(*this == b);
	}

	/*******************************************************************************/

	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator*(const int& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) *= FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(b); }

	inline bool operator > (const int& b) const
	{
		return *this > FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(b);
	}

	inline bool operator < (const int& b) const
	{
		return *this < FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(b);
	}

	inline bool operator <= (const int& b) const
	{
		return *this <= FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(b);
	}

	inline bool operator >= (const int& b) const
	{
		return *this >= FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(b);
	}

	inline bool operator == (const int& b) const
	{
		return *this == FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(b);
	}

	inline bool operator != (const int& b) const
	{
		return *this != FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(b);
	}
};

using tFixedPoint8 = FixedPoint<int, 8, s64>;
using tFixedPoint16 = FixedPoint<int, 16, s64>;
using tFixedPoint24 = FixedPoint<int, 24, s64>;
