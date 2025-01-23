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
	TIntergral m_storage = 0;

	static_assert(sizeof(TIntergral) <= sizeof(TIntermediate), "Intermediate type must be equal to or greater than storage type");

	inline TIntergral GetIntStorage() const
	{
		return m_storage >> FRACTIONAL_BITS;
	}

	inline TIntergral GetFloatStorage() const
	{
		return m_storage & BITS_U32(FRACTIONAL_BITS);
	}

	// Loses float precision easily
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& MulHalfShift(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) {
		m_storage = (static_cast<int>(m_storage) >> (FRACTIONAL_BITS / 2)) * (b.m_storage >> (FRACTIONAL_BITS / 2));
		return *this;
	}

	// High chance of encountering overflow
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& Mul(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) {
		m_storage = (static_cast<int>(m_storage) * b.m_storage) >> FRACTIONAL_BITS;
		return *this;
	}

public:
	inline FixedPoint() : m_storage(0)
	{
	}

	inline FixedPoint(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& that)
	{
		*this = that;
	}

	inline TIntergral GetStorage() const
	{
		return m_storage;
	}

	inline void SetStorage(TIntergral val) volatile
	{
		m_storage = val;
	}

	template<class T, u8 BITS, class V>
	FixedPoint(const FixedPoint<T, BITS, V>& that)
	{
		// Todo- Handle signed and unsigned types
		int shiftDir = static_cast<int>(FRACTIONAL_BITS) - BITS;

		if (shiftDir > 0)
			m_storage = static_cast<TIntergral>(that.GetStorage() << shiftDir);
		else
			m_storage = static_cast<TIntergral>(that.GetStorage() >> -shiftDir);
	}


	static constexpr TIntergral FloatCompress(float val)
	{
		return TIntergral(val * (1 << FRACTIONAL_BITS) + 0.5f);
	}

	static constexpr float FloatDecompress(TIntergral val)
	{
		return (1.0f / static_cast<float>(static_cast<TIntergral>(1) << FRACTIONAL_BITS)) * static_cast<int>(val);
	}

	constexpr inline FixedPoint(int val) : m_storage(static_cast<TIntergral>(val) << FRACTIONAL_BITS) {}
	constexpr inline FixedPoint(float val) : m_storage(FloatCompress(val)) {}
	constexpr inline FixedPoint(double val) : m_storage(static_cast<TIntergral>(val * (1 << FRACTIONAL_BITS) + 0.5)) {}


private:
	constexpr inline int ToInt() const
	{
		return static_cast<int>(GetIntStorage());
	}

	constexpr inline float ToFloat() const
	{
		return FloatDecompress(m_storage);
	}

	constexpr inline double ToDouble() const
	{
		return (1.0 / static_cast<double>(1 << FRACTIONAL_BITS)) * (static_cast<int>(m_storage));
	}

public:
	constexpr inline int ToRoundedInt() const
	{
		return static_cast<int>((m_storage + (1 << FRACTIONAL_BITS) / 2) >> FRACTIONAL_BITS);
	}
	static constexpr inline u8 GetFpLevel() { return FRACTIONAL_BITS; }

	constexpr inline explicit operator int() const { return ToInt(); }
	constexpr inline explicit operator float() const { return ToFloat(); }
	constexpr inline explicit operator double() const { return ToDouble(); }

	/*******************************************************************************/

	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& operator += (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b)
	{
		m_storage += b.m_storage;
		return *this;
	}
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& operator -= (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b)
	{
		m_storage -= b.m_storage;
		return *this;
	}

	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& operator *= (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b)
	{
		return MulHalfShift(b);
	}

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& operator /= (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b)
	{
		// Intermediate type used to handle high likelyhood of overflow here overwise
		constexpr TIntermediate fpOne = static_cast<TIntermediate>(1) << FRACTIONAL_BITS;
		TIntermediate mul = m_storage * fpOne;

		DEBUG_ASSERTMSG(mul != 0 && mul / m_storage == fpOne, "Overflow detected during fixed point division");

		m_storage = static_cast<TIntergral>(mul / b.m_storage);
		return *this;
	}

	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator+(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) += b; }
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator-(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) -= b; }
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator*(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) *= b; }

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate> operator/(const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const { return FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>(*this) /= b; }

	inline bool operator > (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const
	{
		return m_storage > b.m_storage;
	}

	inline bool operator < (const FixedPoint<TIntergral, FRACTIONAL_BITS, TIntermediate>& b) const
	{
		return m_storage < b.m_storage;
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
		return m_storage == b.m_storage;
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

using FPi8 = FixedPoint<int, 8, s64>;
using FPi16 = FixedPoint<int, 16, s64>;
using FPi24 = FixedPoint<int, 24, s64>;
