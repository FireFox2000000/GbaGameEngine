#pragma once
#include "../../Typedefs.h"
#include "../../Macros.h"
#include "../Memory.h"

template<class T, u32 size>
class FixedArray
{
	T buffer[size];

public:
	FixedArray();
	FixedArray(const FixedArray<T, size> & that) { *this = that; }
	~FixedArray() {}

	inline u32 Count() const { return size; }
	inline u32 Count() volatile { return size; }

	inline void operator = (const FixedArray<T, size>& that) { MemCopy(&that, this, sizeof(T) * Count()); }

	inline void operator = (const FixedArray<T, size>& that) volatile
	{
		for (u32 i = 0; i < size; ++i)
		{
			buffer[i] = that.buffer[i];
		}
	}

	inline T & operator[](u32 index) { return buffer[index]; }
	inline const T & operator[](u32 index) const { return buffer[index]; }
	inline volatile T & operator[](u32 index) volatile { return buffer[index]; }
	
	inline T * At(u32 index);
	inline const T * At(u32 index) const;
	inline volatile T * At(u32 index) volatile;
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

template<class T, u32 size>
inline T* FixedArray<T, size>::At(u32 index) {
	return index < Count() ? &buffer[index] : NULL;
}

template<class T, u32 size>
inline volatile T* FixedArray<T, size>::At(u32 index) volatile {
	return index < Count() ? &buffer[index] : NULL;
}

template<class T, u32 size>
inline const T* FixedArray<T, size>::At(u32 index) const {
	return index < Count() ? &buffer[index] : NULL;
}
