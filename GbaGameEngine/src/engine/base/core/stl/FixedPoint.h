#pragma once
#include <type_traits>	// for is_integral
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include <concepts>

template<std::integral TBaseStorage, u8 FRACTIONAL_BITS>
class FixedPoint 
{
	TBaseStorage storage;

	inline TBaseStorage GetIntStorage() const
	{
		return storage >> FRACTIONAL_BITS;
	}

	inline TBaseStorage GetFloatStorage() const
	{
		return storage & BITS_U32(FRACTIONAL_BITS);
	}

	// Looses float precision easily
	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS>& MulHalfShift(const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) {
		storage = ((int)storage >> (FRACTIONAL_BITS / 2)) * (b.storage >> (FRACTIONAL_BITS / 2));
		return *this;
	}

	// High chance of encountering overflow
	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS>& Mul(const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) {
		storage = ((int)storage * b.storage) >> FRACTIONAL_BITS;
		return *this;
	}

public:
	inline FixedPoint() : storage(0)
	{
	}

	inline FixedPoint(const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& that)
	{
		*this = that;
	}

	inline TBaseStorage GetStorage() const
	{
		return storage;
	}

	inline void SetStorage(TBaseStorage val) volatile
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


	static constexpr TBaseStorage FloatCompress(float val)
	{
		return TBaseStorage(val * (1 << FRACTIONAL_BITS) + 0.5f);
	}

	static constexpr float FloatDecompress(TBaseStorage val)
	{
		return (1.0f / (float)(1 << FRACTIONAL_BITS)) * ((int)val);
	}

	constexpr inline FixedPoint(int val) : storage(val << FRACTIONAL_BITS) {}
	constexpr inline FixedPoint(float val) : storage(FloatCompress(val)) {}
	constexpr inline FixedPoint(double val) : storage(TBaseStorage(val * (1 << FRACTIONAL_BITS) + 0.5)) {}

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

	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS>& operator += (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b)
	{
		storage += b.storage;
		return *this;
	}
	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS>& operator -= (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b)
	{
		storage -= b.storage;
		return *this;
	}

	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS>& operator *= (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b)
	{
		return MulHalfShift(b);
	}

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS>& operator /= (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b)
	{
		storage = storage * (1 << FRACTIONAL_BITS) / b.storage;
		return *this;
	}

	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS> operator+(const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const { return FixedPoint<TBaseStorage, FRACTIONAL_BITS>(*this) += b; }
	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS> operator-(const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const { return FixedPoint<TBaseStorage, FRACTIONAL_BITS>(*this) -= b; }
	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS> operator*(const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const { return FixedPoint<TBaseStorage, FRACTIONAL_BITS>(*this) *= b; }

	// Easy to overflow and underflow. Try not to use this if it can be helped
	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS> operator/(const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const { return FixedPoint<TBaseStorage, FRACTIONAL_BITS>(*this) /= b; }

	inline bool operator > (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const
	{
		return storage > b.storage;
	}

	inline bool operator < (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const
	{
		return storage < b.storage;
	}

	inline bool operator <= (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const
	{
		return !(*this > b);
	}

	inline bool operator >= (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const
	{
		return !(*this < b);
	}

	inline bool operator == (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const
	{
		return storage == b.storage;
	}

	inline bool operator != (const FixedPoint<TBaseStorage, FRACTIONAL_BITS>& b) const
	{
		return !(*this == b);
	}

	/*******************************************************************************/

	inline FixedPoint<TBaseStorage, FRACTIONAL_BITS> operator*(const int& b) const { return FixedPoint<TBaseStorage, FRACTIONAL_BITS>(*this) *= FixedPoint<TBaseStorage, FRACTIONAL_BITS>(b); }

	inline bool operator > (const int& b) const
	{
		return *this > FixedPoint<TBaseStorage, FRACTIONAL_BITS>(b);
	}

	inline bool operator < (const int& b) const
	{
		return *this < FixedPoint<TBaseStorage, FRACTIONAL_BITS>(b);
	}

	inline bool operator <= (const int& b) const
	{
		return *this <= FixedPoint<TBaseStorage, FRACTIONAL_BITS>(b);
	}

	inline bool operator >= (const int& b) const
	{
		return *this >= FixedPoint<TBaseStorage, FRACTIONAL_BITS>(b);
	}

	inline bool operator == (const int& b) const
	{
		return *this == FixedPoint<TBaseStorage, FRACTIONAL_BITS>(b);
	}

	inline bool operator != (const int& b) const
	{
		return *this != FixedPoint<TBaseStorage, FRACTIONAL_BITS>(b);
	}
};

using tFixedPoint8 = FixedPoint<int, 8>;
using tFixedPoint16 = FixedPoint<int, 16>;
using tFixedPoint24 = FixedPoint<int, 24>;
