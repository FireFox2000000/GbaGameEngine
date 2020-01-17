#pragma once
#include <type_traits>	// for is_integral
#include "engine/base/Macros.h"

template<class T>
class Bitmask
{
	T m_storage;

public:
	inline Bitmask()
	{
		STATIC_ASSERT(std::is_integral<T>::value, "Integral required.");
		m_storage = 0;
	}

	inline Bitmask(T mask)
	{
		STATIC_ASSERT(std::is_integral<T>::value, "Integral required.");
		m_storage = mask;
	}

	inline void SetBit(int index)
	{
		m_storage |= BIT(index);
	}

	inline void ClearBit(int index)
	{
		m_storage &= ~BIT(index);
	}

	inline bool TestBit(int index) const
	{
		return (m_storage & BIT(index)) != 0;
	}

	inline bool TestMask(const Bitmask<T>& that) const
	{
		return (m_storage & that.m_storage) != 0;
	}

	inline void SetMask(T mask)
	{
		m_storage = mask;
	}

	template<class V>
	void SetValue(V value, int bitStartIndex, int length)
	{
		int mask = BITS_INDEXED_U32(length, bitStartIndex);
		T storedValue = SHIFTED_BITMASK(value, bitStartIndex);
		m_storage = (m_storage & ~mask) | (storedValue & mask);
	}

	template<class V>
	V GetValue(int bitStartIndex, int length) const
	{
		int mask = BITS_INDEXED_U32(length, bitStartIndex);
		T storedValue = m_storage & ~mask;
		return (V)(storedValue >> bitStartIndex);
	}
};
