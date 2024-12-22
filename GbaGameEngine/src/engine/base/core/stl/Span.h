#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

template<class T>
class Span
{
	T* m_data = nullptr;
	u32 m_size = 0;

	inline T& Get(u32 index) 
	{
		DEBUG_ASSERTMSG(index <= Count(), "Index out of bounds of span");
		return m_data[index]; 
	}

	inline const T& Get(u32 index) const 
	{ 
		DEBUG_ASSERTMSG(index <= Count(), "Index out of bounds of span");
		return m_data[index]; 
	}

	inline volatile T& Get(u32 index) volatile 
	{ 
		DEBUG_ASSERTMSG(index <= Count(), "Index out of bounds of span");
		return m_data[index]; 
	}

public:
	Span() = default;
	Span(T* data, u32 size)
		: m_data(data)
		, m_size(size)
	{}

	inline u32 Count() const { return m_size; }
	inline bool Empty() const { return m_size == 0; }
	inline T* Data() const { return m_data; }

	using iterator = T*;
	using const_iterator = const T*;
	using volatile_iterator = volatile T*;

	iterator begin() { return &Get(0); }
	const_iterator begin() const { return &Get(0); }
	volatile_iterator begin() volatile { return &Get(0); }

	iterator end() { return &Get(Count()); }
	const_iterator end() const { return &Get(Count()); }
	volatile_iterator end() volatile { return &Get(Count()); }

	inline T& operator[](u32 index) { return Get(index); }
	inline const T& operator[](u32 index) const { return Get(index); }
	inline volatile T& operator[](u32 index) volatile { return Get(index); }

	Span<T> Slice(int start, int length) const
	{
		DEBUG_ASSERTMSG(static_cast<u32>(start + length) <= Count(), "Slice is malformed");
		return { m_data + start, static_cast<u32>(length) };
	}
};
