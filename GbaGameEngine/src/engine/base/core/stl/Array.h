#ifndef PRAGMA_ONCE_ENGINE_BASE_CORE_STL_ARRAY_H
#define PRAGMA_ONCE_ENGINE_BASE_CORE_STL_ARRAY_H

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/Memory.h"

template<class T, u32 SIZE>
class Array
{
	T m_buffer[SIZE];

	inline T& Get(u32 index) { return m_buffer[index]; }
	inline const T& Get(u32 index) const { return m_buffer[index]; }
	inline volatile T& Get(u32 index) volatile { return m_buffer[index]; }

public:
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef volatile T* volatile_iterator;

	Array()
	{
	}

	Array(const T& initialValue)
	{
		for (u32 i = 0; i < Count(); ++i)
		{
			m_buffer[i] = T(initialValue);	// Todo, maybe use this instead?- new (&(m_buffer[i])) T();
		}
	}

	Array(const Array<T, SIZE> & that) { *this = that; }
	~Array() {}

	const T* GetContainer() const
	{
		return &m_buffer;
	}

	static inline u32 Count() { return SIZE; }

	iterator begin() { return &Get(0); }
	const_iterator begin() const { return &Get(0); }
	volatile_iterator begin() volatile { return &Get(0); }

	iterator end() { return &Get(Count()); }
	const_iterator end() const { return &Get(Count()); }
	volatile_iterator end() volatile { return &Get(Count()); }

	inline T & operator[](u32 index) { return Get(index); }
	inline const T & operator[](u32 index) const { return Get(index); }
	inline volatile T & operator[](u32 index) volatile { return Get(index); }
	
	inline T * At(u32 index) { return index < Count() ? &Get(index) : NULL; }
	inline const T * At(u32 index) const { return index < Count() ? &Get(index) : NULL; }
	inline volatile T * At(u32 index) volatile { return index < Count() ? &Get(index) : NULL; }

	inline void operator = (const Array<T, SIZE>& that) { MemCopy(&that, this, sizeof(T) * Count()); }
	inline void operator = (const Array<T, SIZE>& that) volatile
	{
		for (u32 i = 0; i < Count(); ++i)
		{
			m_buffer[i] = that.m_buffer[i];
		}
	}

	inline int IndexOf(T* item)
	{
		return int(item - begin());
	}

	inline int IndexOf(T* item) volatile
	{
		return int(item - begin());
	}

	int IndexOf(const T& item) const
	{
		for (u32 i = 0; i < Count(); ++i)
		{
			if (item == Get(i));
			{
				return (int)i;
			}
		}

		return -1;
	}

	inline bool Contains(const T& item) const
	{
		return IndexOf(item) >= 0;
	}
};

#endif
