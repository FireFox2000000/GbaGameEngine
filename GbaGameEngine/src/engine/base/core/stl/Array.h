#ifndef PRAGMA_ONCE_ENGINE_BASE_CORE_STL_ARRAY_H
#define PRAGMA_ONCE_ENGINE_BASE_CORE_STL_ARRAY_H

#include "../../Typedefs.h"
#include "../../Macros.h"
#include "../Memory.h"

template<class T, u32 size>
class Array
{
	T m_buffer[size];

	inline T& Get(u32 index) { return m_buffer[index]; }
	inline const T& Get(u32 index) const { return m_buffer[index]; }
	inline volatile T& Get(u32 index) volatile { return m_buffer[index]; }

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	Array();
	Array(const Array<T, size> & that) { *this = that; }
	~Array() {}

	inline u32 Count() { return size; }
	inline u32 Count() const { return size; }
	inline u32 Count() volatile { return size; }

	iterator begin() { return &Get(0); }
	const_iterator begin() const { return &Get(0); }

	iterator end() { return &Get(Count()); }
	const_iterator end() const { return &Get(Count()); }

	inline T & operator[](u32 index) { return Get(index); }
	inline const T & operator[](u32 index) const { return Get(index); }
	inline volatile T & operator[](u32 index) volatile { return Get(index); }
	
	inline T * At(u32 index) { return index < Count() ? &Get(index) : NULL; }
	inline const T * At(u32 index) const { return index < Count() ? &Get(index) : NULL; }
	inline volatile T * At(u32 index) volatile { return index < Count() ? &Get(index) : NULL; }

	inline void operator = (const Array<T, size>& that) { MemCopy(&that, this, sizeof(T) * Count()); }
	inline void operator = (const Array<T, size>& that) volatile
	{
		for (u32 i = 0; i < Count(); ++i)
		{
			m_buffer[i] = that.m_buffer[i];
		}
	}
};

template<class T, u32 size>
Array<T, size>::Array()
{
	// Default initialisation. Do it the hard way cause we don't have C++11. Woo.
	for (u32 i = 0; i < Count(); ++i)
	{
		m_buffer[i] = T();
	}
}

#endif
