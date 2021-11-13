#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/Memory.h"
#include <initializer_list>

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
		SetAllTo(initialValue);
	}

	Array(const Array<T, SIZE> & that) { *this = that; }

	Array(const std::initializer_list<T>& l)
	{
		u32 i = 0;
		for (auto& val : l)
		{
			m_buffer[i] = T(val);
			if (++i >= Count())
			{
				break;
			}
		}
	}

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

	inline T * At(u32 index) { DEBUG_ASSERTMSG(index < Count(), "Array index out of range"); return &Get(index); }
	inline const T * At(u32 index) const { DEBUG_ASSERTMSG(index < Count(), "Array index out of range"); return &Get(index); }
	inline volatile T * At(u32 index) volatile { DEBUG_ASSERTMSG(index < Count(), "Array index out of range"); return &Get(index); }

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
			if (item == Get(i))
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

	void SetAllTo(const T& val)
	{
		for (u32 i = 0; i < Count(); ++i)
		{
			m_buffer[i] = T(val);	// Todo, maybe use this instead?- new (&(m_buffer[i])) T();
		}
	}
};

template<class T>
class Array<T, 0>
{
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef volatile T* volatile_iterator;

	static inline u32 Count() { return 0; }

	iterator begin() { return nullptr; }
	const_iterator begin() const { return nullptr; }
	volatile_iterator begin() volatile { return nullptr; }

	iterator end() { return nullptr; }
	const_iterator end() const { return nullptr; }
	volatile_iterator end() volatile { return nullptr; }
};
