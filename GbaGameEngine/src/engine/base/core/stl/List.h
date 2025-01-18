#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/Memory.h"
#include "engine/math/Math.h"
#include <initializer_list>

// Includes List<> and FixedList<>
// List<> is basically the same as std::vector, but I don't really like the weird naming and so this is inspired to read more like C#'s list class. 
// FixedList<> is the same as List but with a fixed allocation size. Does not use heap memory, allocated via array. 

template<class T>
class MAllocMemoryPolicy
{
	T* m_container;
	u32 m_capacity;
protected:
	MAllocMemoryPolicy(u32 size)
		: m_capacity(MAX(size, u32(1)))
	{
		m_container = MAllocType<T>(size);
		m_capacity = size;
	}

	~MAllocMemoryPolicy()
	{
		SafeFree(m_container);
	}

	MAllocMemoryPolicy(MAllocMemoryPolicy&& other)
		: m_container(other.m_container)
		, m_capacity(other.m_capacity)
	{
		other.m_container = nullptr;
		other.m_capacity = 0;
	}

	MAllocMemoryPolicy& operator=(MAllocMemoryPolicy&& other)
	{
		if (this != &other)
		{
			SafeFree(m_container);

			m_container = other.m_container;
			m_capacity = other.m_capacity;

			other.m_container = nullptr;
			other.m_capacity = 0;
		}

		return *this;
	}

	bool Reallocate(u32 size, u32 count)
	{
		static_assert(std::is_trivially_copyable<T>::value, "Must be trivially constructable");

		T* newContainer = MAllocType<T>(size);
		if (newContainer)
		{
			MemCopy(newContainer, m_container, sizeof(T) * MIN(count, size));
			SafeFree(m_container);

			m_container = newContainer;
			m_capacity = size;
			return true;
		}

		return false;
	}

	inline T& Get(u32 index) { return m_container[index]; }
	inline const T& Get(u32 index) const { return m_container[index]; }

	inline u32 Capacity() { return m_capacity; }
	inline u32 Capacity() const { return m_capacity; }

	inline T* GetContainer() { return m_container; }
};

template<class T, u32 SIZE>
class FixedMemoryPolicy
{
	// Define u8 as we're going to explicitly handle constructor and destructor calls, don't want array to call them itself. 
	alignas(T) u8 m_container[SIZE * sizeof(T)];

protected:
	FixedMemoryPolicy(u32 size)
	{
	}

	~FixedMemoryPolicy()
	{
	}

	bool Reallocate(u32 size, u32 count)
	{
		// No.
		DEBUG_ASSERTMSG(size <= SIZE, "FixedList out of memory");
		return size <= SIZE;
	}

	inline T& Get(u32 index) { return (T&)(m_container[index * sizeof(T)]); }
	inline const T& Get(u32 index) const { return (T&)(m_container[index * sizeof(T)]); }

	inline u32 Capacity() { return SIZE; }
	inline u32 Capacity() const { return SIZE; }

	inline T* GetContainer() { return (T*)(m_container); }
};

template<class T>
class FixedMemoryPolicy<T, 0>
{
	u8* m_container = nullptr;

protected:
	FixedMemoryPolicy(u32 size)
	{
	}

	~FixedMemoryPolicy()
	{
	}

	bool Reallocate(u32 size, u32 count)
	{
		// No.
		DEBUG_ERROR("FixedList out of memory");
		return false;
	}

	inline T& Get(u32 index) { DEBUG_ERROR("FixedList of size 0 cannot be accessed."); return (T&)(m_container[0]); }
	inline const T& Get(u32 index) const { DEBUG_ERROR("FixedList of size 0 cannot be accessed."); return (T&)(m_container[0]); }

	inline u32 Capacity() { return 0; }
	inline u32 Capacity() const { return 0; }

	inline T* GetContainer() { return (T*)(m_container); }
};

template<class T, class MemoryPolicy>
class ListBase : public MemoryPolicy
{
	u32 m_count;

	bool GrowTo(u32 size)
	{
		if (MemoryPolicy::Reallocate(size, Count()))
		{
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
			{
				DEBUG_ERROR("Unable to add element. List out of memory");
				return nullptr;
			}
		}

		u32 length = 1;
		u32 endPosition = index + length;
		MoveMemory(MemoryPolicy::GetContainer() + endPosition, MemoryPolicy::GetContainer() + index, sizeof(T) * (Count() - endPosition + length));
		++m_count;

		T* newItem = &Get(index);
		return newItem;
	}

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	ListBase(u32 initialCapacity = 1)
		: MemoryPolicy(initialCapacity)
		, m_count(0)
	{
	}

	ListBase(const ListBase<T, MemoryPolicy>& that) : ListBase<T, MemoryPolicy>()
	{
		*this = that;
	}

	ListBase(ListBase<T, MemoryPolicy>&& that) 
		: MemoryPolicy(std::move(that))
		, m_count(that.m_count)
	{
		that.m_count = 0;
	}

	ListBase<T, MemoryPolicy>& operator=(ListBase<T, MemoryPolicy>&& other)
	{
		if (this != &other)
		{
			MemoryPolicy::operator=(std::move(other));
			other.m_count = 0;
		}

		return *this;
	}

	ListBase(std::initializer_list<T> l) : ListBase<T, MemoryPolicy>(l.size())
	{
		InsertRange(0, l);
	}

	~ListBase() requires std::is_trivially_destructible_v<T> = default;

	~ListBase()
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			it->~T();
		}
	}

	ListBase& operator=(const ListBase<T, MemoryPolicy>& that)
	{
		Reserve(that.Capacity());

		for (ListBase<T, MemoryPolicy>::const_iterator it = that.begin(); it != that.end(); ++it)
		{
			Add(*it);
		}

		return *this;
	}

	inline T& Get(u32 index) { DEBUG_ASSERTMSGFORMAT(index >= 0 && index < Count(), "List index (%d) out of range", index); return MemoryPolicy::Get(index); }
	inline const T& Get(u32 index) const { DEBUG_ASSERTMSGFORMAT(index >= 0 && index < Count(), "List index (%d) out of range", index); return MemoryPolicy::Get(index); }

	inline u32 Count() { return m_count; }
	inline u32 Count() const { return m_count; }

	inline u32 Capacity() { return MemoryPolicy::Capacity(); }
	inline u32 Capacity() const { return MemoryPolicy::Capacity(); }

	inline iterator begin() { return Count() > 0 ? &Get(0) : nullptr; }
	inline const_iterator begin() const { return Count() > 0 ? &Get(0) : nullptr; }

	inline iterator end() { return Count() > 0 ? &MemoryPolicy::Get(Count()) : nullptr; }
	inline const_iterator end() const { return Count() > 0 ? &MemoryPolicy::Get(Count()) : nullptr; }

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

	inline T* InsertAt(u32 index, const T& item)
	{
		T* newItem = AddUninitialisedItemAt(index);
		return new(newItem) T(item);
	}

	inline T* InsertNewAt(u32 index)
	{
		T* newItem = AddUninitialisedItemAt(index);
		return new(newItem) T();
	}

	inline T* AddNew()
	{
		return InsertNewAt(Count());
	}

	template <typename... ConstructorArgs>
	inline T* AddNew(ConstructorArgs... args)
	{
		T* newItem = AddUninitialisedItemAt(Count());
		return new(newItem) T(args ...);
	}

	inline T* Add(const T& item)
	{
		return InsertAt(Count(), item);
	}

	// EnumerableT must implement begin and end iterators
	template<typename EnumerableT>
	bool InsertRange(u32 index, const EnumerableT& items)
	{
		if (index > Count())
		{
			DEBUG_ERROR("List insertion index out of range");
			return false;
		}

		u32 length = u32(items.end() - items.begin());
		u32 totalSizeRequired = Count() + length;
		if (totalSizeRequired > Capacity())
		{
			u32 newSize = Capacity();
			do
			{
				newSize *= 2;
			} while (newSize < totalSizeRequired);

			if (!GrowTo(newSize))
			{
				DEBUG_ERRORFORMAT("List out of memory trying to grow to size %d", newSize);
				return false;
			}
		}

		u32 endPosition = index + length;
		MoveMemory(MemoryPolicy::GetContainer() + endPosition, MemoryPolicy::GetContainer() + index, sizeof(T) * (Count() - endPosition + length));
		m_count += length;

		typename EnumerableT::const_iterator itBegin = items.begin();
		for (u32 i = 0; i < length; ++i)
		{
			T* newItem = &Get(i + index);
			new(newItem) T(*(itBegin + i));
		}

		return true;
	}

	bool RemoveRange(u32 index, u32 length)
	{
		if (length > 0 && index + length <= Count())
		{
			u32 endPosition = index + length;
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				for (u32 i = index; i < endPosition; ++i)
				{
					Get(i).~T();
				}
			}

			MoveMemory(MemoryPolicy::GetContainer() + index, MemoryPolicy::GetContainer() + endPosition, sizeof(T) * (Count() - endPosition));
			m_count -= length;
			return true;
		}
		else
		{
			return false;
		}
	}

	inline bool RemoveAt(u32 index)
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

	inline bool Clear()
	{
		return RemoveRange(0, Count());
	}

	inline const T* GetContainer() { return MemoryPolicy::GetContainer(); }
};

template<class T>
class List : public ListBase<T, MAllocMemoryPolicy<T> >
{
	using ListBase<T, MAllocMemoryPolicy<T> >::ListBase;
};

template<class T, u32 SIZE>
class FixedList : public ListBase<T, FixedMemoryPolicy<T, SIZE> >
{
	using ListBase<T, FixedMemoryPolicy<T, SIZE> >::ListBase;
};
