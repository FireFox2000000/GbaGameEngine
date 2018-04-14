#ifndef PRAGMA_ONCE_ENGINE_BASE_CORE_STL_LIST_H
#define PRAGMA_ONCE_ENGINE_BASE_CORE_STL_LIST_H

#include "../../Typedefs.h"
#include "../../Macros.h"
#include "../Memory.h"

template<class T>
class List
{
	T* m_container;
	u32 m_capacity;
	u32 m_count;

	inline T& Get(u32 index) { return m_container[index]; }
	inline const T& Get(u32 index) const { return m_container[index]; }

	bool GrowTo(u32 size)
	{
		T* newContainer = New<T>(size);
		if (newContainer)
		{
			MemCopy(m_container, newContainer, sizeof(T) * MIN(Count(), size));
			Delete(m_container);

			m_container = newContainer;
			m_capacity = size;
			if (Count() > size)
				m_count = size;

			return true;
		}

		return false;
	}

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	List(u32 initialCapacity = 1) : m_capacity(MAX(initialCapacity, u32(1))), m_count(0)
	{
		m_container = New<T>(m_capacity);
	}

	~List()
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			it->~T();
		}
		Delete(m_container);
	}

	inline u32 Count() { return m_count; }
	inline u32 Count() const { return m_count; }

	inline u32 Capacity() { return m_capacity; }
	inline u32 Capacity() const { return m_capacity; }

	iterator begin() { return Count() > 0 ? &Get(0) : NULL; }
	const_iterator begin() const { return Count() > 0 ? &Get(0) : NULL; }

	iterator end() { return Count() > 0 ? &Get(Count()) : NULL; }
	const_iterator end() const { return Count() > 0 ? &Get(Count()) : NULL; }

	inline T & operator[](u32 index) { return Get(index); }
	inline const T & operator[](u32 index) const { return Get(index); }

	bool Reserve(u32 size)
	{
		return GrowTo(size);
	}

	T* AddNew()
	{
		if (Count() >= Capacity())
		{
			if (!GrowTo(Capacity() * 2))
				return NULL;
		}

		T& item = Get(m_count++);
		new(&item) T();
		return &item;
	}

	T* AddCopy(const T& item)
	{
		if (Count() >= Capacity())
		{
			if (!GrowTo(Capacity() * 2))
				return NULL;
		}

		T& newItem = Get(m_count++);
		new(&newItem) T(item);
		return &newItem;
	}

	/*
	// C++11 Parameter Pack version
	template <typename... ConstructorArgs>
	T* AddNew(ConstructorArgs... args)
	{
	if (Count() >= Capacity())
	{
	if (!GrowTo(Capacity() * 2))
	return NULL;
	}

	T& item = Get(m_count++);
	item = T(args ...);
	return &item;
	}*/

	void RemoveRange(u32 position, u32 length)
	{
		if (length > 0 && position + length <= Count())
		{
			u32 endPosition = position + length;
			for (u32 i = position; i < endPosition; ++i)
			{
				Get(i).~T();
			}

			MoveMemory(m_container + position, m_container + endPosition, sizeof(T) * (Count() - endPosition));
			m_count -= length;
		}
	}

	void RemoveAt(u32 position)
	{
		RemoveRange(position, 1);
	}

	void Remove(T* item)
	{
		int position = int(item - m_container);
		if (position >= 0)
			RemoveAt(position);
	}
};

#endif
