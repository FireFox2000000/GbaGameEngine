#pragma once
#include "../../Typedefs.h"
#include "../../Macros.h"
#include "../Memory.h"

template<class T, u32 size>
class FixedArray
{
	T buffer[size];

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	FixedArray();
	FixedArray(const FixedArray<T, size> & that) { *this = that; }
	~FixedArray() {}

	inline u32 Count() { return size; }
	inline u32 Count() const { return size; }
	inline u32 Count() volatile { return size; }

	iterator begin() { return &buffer[0]; }
	const_iterator begin() const { return &buffer[0]; }

	iterator end() { return &buffer[Count()]; }
	const_iterator end() const { return &buffer[Count()]; }

	inline T & operator[](u32 index) { return buffer[index]; }
	inline const T & operator[](u32 index) const { return buffer[index]; }
	inline volatile T & operator[](u32 index) volatile { return buffer[index]; }
	
	inline T * At(u32 index) { return index < Count() ? &buffer[index] : NULL; }
	inline const T * At(u32 index) const { return index < Count() ? &buffer[index] : NULL; }
	inline volatile T * At(u32 index) volatile { return index < Count() ? &buffer[index] : NULL; }

	inline void operator = (const FixedArray<T, size>& that) { MemCopy(&that, this, sizeof(T) * Count()); }
	inline void operator = (const FixedArray<T, size>& that) volatile
	{
		for (u32 i = 0; i < size; ++i)
		{
			buffer[i] = that.buffer[i];
		}
	}
};

template<class T, u32 size>
FixedArray<T, size>::FixedArray()
{
	// Default initialisation. Do it the hard way cause we don't have C++11. Woo.
	for (u32 i = 0; i < size; ++i)
	{
		buffer[i] = T();
	}
}
