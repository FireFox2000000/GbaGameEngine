#ifndef PRAGMA_ONCE_ENGINE_BASE_CORE_STL_LIST_H
#define PRAGMA_ONCE_ENGINE_BASE_CORE_STL_LIST_H

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/Memory.h"

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
		T* newContainer = MAllocType<T>(size);
		if (newContainer)
		{
			MemCopy(m_container, newContainer, sizeof(T) * MIN(Count(), size));
			SafeFree(m_container);

			m_container = newContainer;
			m_capacity = size;
			if (Count() > size)
				m_count = size;

			return true;
		}

		return false;
	}

	T* AddUninitialisedItemAt(u32 index)
	{
		if (Count() >= Capacity())
		{
			if (!GrowTo(Capacity() * 2))
				return NULL;
		}

		u32 length = 1;
		u32 endPosition = index + length;
		MoveMemory(m_container + endPosition, m_container + index, sizeof(T) * (Count() - endPosition + length));
		++m_count;

		T* newItem = &Get(index);
		return newItem;
	}

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	List(u32 initialCapacity = 1) : m_capacity(MAX(initialCapacity, u32(1))), m_count(0)
	{
		m_container = MAllocType<T>(m_capacity);
	}

	List(const List<T>& that) : List<T>()
	{
		Reserve(that.Capacity());
		for (List<T>::const_iterator it = that.begin(); it != that.end(); ++it)
		{
			Add(*it);
		}
	}

	~List()
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			it->~T();
		}
		SafeFree(m_container);
	}

	const T* GetContainer() const
	{
		return m_container;
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
		if (size > Capacity())
			return GrowTo(size);
		else
			return true; // ???
	}

	inline int IndexOf(T* item)
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

	T* InsertAt(u32 index, const T& item)
	{
		T* newItem = AddUninitialisedItemAt(index);
		return new(newItem) T(item);
	}

	T* InsertNewAt(u32 index)
	{
		T* newItem = AddUninitialisedItemAt(index);
		return new(newItem) T();
	}

	T* AddNew()
	{
		return InsertNewAt(Count());
	}

	T* Add(const T& item)
	{
		return InsertAt(Count(), item);
	}

	// EnumerableT must implement begin and end iterators
	template<typename EnumerableT>
	bool InsertRange(u32 index, const EnumerableT& items)
	{
		if (index > Count())
			return false;

		u32 length = u32(items.end() - items.begin());
		u32 totalSizeRequired = Count() + length;
		if (totalSizeRequired >= Capacity())
		{
			u32 newSize = Capacity();
			do
			{
				newSize *= 2;
			} while (newSize < totalSizeRequired);

			if (!GrowTo(newSize))
				return false;
		}

		u32 endPosition = index + length;
		MoveMemory(m_container + endPosition, m_container + index, sizeof(T) * (Count() - endPosition + length));

		typename EnumerableT::const_iterator itBegin = items.begin();
		for (u32 i = 0; i < length; ++i)
		{
			T* newItem = &Get(i + index);
			new(newItem) T(*(itBegin + i));
		}

		m_count += length;
		return true;
	}

	bool RemoveRange(u32 index, u32 length)
	{
		if (length > 0 && index + length <= Count())
		{
			u32 endPosition = index + length;
			for (u32 i = index; i < endPosition; ++i)
			{
				Get(i).~T();
			}

			MoveMemory(m_container + index, m_container + endPosition, sizeof(T) * (Count() - endPosition));
			m_count -= length;
			return true;
		}
		else
			return false;
	}

	bool RemoveAt(u32 index)
	{
		return RemoveRange(index, 1);
	}

	bool Remove(T* item)
	{
		int index = IndexOf(item);
		if (index >= 0)
			return RemoveAt(index);
		else
			return false;
	}

	bool Clear()
	{
		return RemoveRange(0, Count());
	}
};

#endif
