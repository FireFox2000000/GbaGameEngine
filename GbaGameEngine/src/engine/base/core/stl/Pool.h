// https://www.gamasutra.com/blogs/MateusGondimLima/20180720/322503/Designing_and_implementing_a_pool_allocator_data_structure_for_memory_management_in_games.php
// Basically an array, but unused elements store pointer addresses in a linked-list fashion to the rest of the free elements
// Has automatic cleanup of currently active objects that have no been free upon destruction. 

// ALso probably worth looking at https://gameprogrammingpatterns.com/object-pool.html as well

#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

template<class T, int SIZE>
class Pool
{
	struct PoolObject
	{
		// Have this as the first item in the struct so we can cast T* to PoolObject*
		union
		{
			PoolObject* next;
			T object;
		};

		// Basically an "in use" flag to determine if "object" is live or not to figure out whether to call destrutor on object for an auto-free
		bool active = false;	// This may be better in SOA alignment for iterator speed when pools are mostly empty. 
	};

	// Create a u8 array instead of PoolObject array to get around union implicitly deleting destructor error
	// We'll handle the destructor calls ourselves. 
	alignas(PoolObject) u8 m_objects[sizeof(PoolObject) * SIZE];
	PoolObject* m_nextFree;

	T* AllocUninitialisedItem()
	{
		// Retrieve the next free item
		m_nextFree->active = true;
		T* newItem = &m_nextFree->object;

		// Update the next free item in the list
		PoolObject* nextFree = m_nextFree->next;
		m_nextFree = nextFree;

		return newItem;
	}

	PoolObject* GetAt(int index)
	{
		PoolObject* objects = reinterpret_cast<PoolObject*>(&m_objects[0]);
		return objects + index;
	}

	const PoolObject* GetAt(int index) const
	{
		const PoolObject* objects = reinterpret_cast<const PoolObject*>(&m_objects[0]);
		return objects + index;
	}

public:
	Pool()
	{
		// To create the free list, we first initialize the head of the list, setting its value to the starting address of the poolÅfs memory block.
		// Next, using the elementÅfs size in bytes and the number of elements in the pool, we calculate the address of the last element.
		// Finally, knowing the number of elements and the last elementÅfs address, we iterate through each element in the pool, 
		// writing the address of the next element in its memory block, with the exception of the last element, whose next pointer value is set to null.

		PoolObject* objects = reinterpret_cast<PoolObject*>(&m_objects[0]);
		for (int i = 0; i < SIZE; ++i)
		{
			PoolObject* current = objects + i;
			int next = i + 1;
			if (next >= SIZE)
			{
				current->next = nullptr;
			}
			else
			{
				current->next = objects + next;
			}

			current->active = false;
		}

		m_nextFree = objects;
	}

	class iterator_base
	{
	protected:
		PoolObject* m_current;
		PoolObject* m_end;

		void Advance()
		{
			do
			{
				++m_current;
			} while (m_current < m_end && !m_current->active);
		}

		inline T& Get()
		{
			return m_current->object;
		}

		inline const T& Get() const
		{
			return m_current->object;
		}

	public:
		iterator_base(PoolObject* current, PoolObject* end) : m_current(current), m_end(end)
		{
			if (m_current < m_end && !m_current->active)
			{
				Advance();
			}
		}

		bool operator == (const iterator_base& that)
		{
			return this->m_current == that.m_current;
		}

		bool operator != (const iterator_base& that)
		{
			return !(*this == that);
		}
	};

	class iterator : public iterator_base
	{
	public:
		iterator(PoolObject* current, PoolObject* end) : iterator_base(current, end) {}

		inline T& operator * () { return iterator_base::Get(); }
		inline T* operator -> () { return &iterator_base::Get(); }

		inline iterator& operator ++ () {
			iterator_base::Advance();
			return *this;
		}
	};

	class const_iterator : public iterator_base
	{
	public:
		const_iterator(PoolObject* current, PoolObject* end) : iterator_base(current, end) {}

		inline const T& operator * () const { return iterator_base::Get(); }
		inline const T* operator -> () const { return &iterator_base::Get(); }

		inline const_iterator& operator ++ () {
			iterator_base::Advance();
			return *this;
		}
	};

	~Pool()
	{
		// Safe free all
		int freeCount = 0;

		PoolObject* objects = reinterpret_cast<PoolObject*>(&m_objects[0]);
		for (int i = 0; i < SIZE; ++i)
		{
			PoolObject* current = objects + i;
			if (current->active)
			{
				Free(&current->object);
				++freeCount;
			}
		}

		DEBUG_LOGFORMAT("Pool automatically freed %d objects that were still in use.", freeCount);

		m_nextFree = nullptr;
	}

	iterator begin() { return iterator(GetAt(0), GetAt(SIZE)); }
	const_iterator begin() const { return const_iterator(const_cast<PoolObject*>(GetAt(0)), const_cast<PoolObject*>(GetAt(SIZE))); }

	iterator end() { return iterator(GetAt(SIZE), GetAt(SIZE)); }
	const_iterator end() const { return const_iterator(const_cast<PoolObject*>(GetAt(SIZE)), const_cast<PoolObject*>(GetAt(SIZE))); }

	inline bool IsFull() { return !m_nextFree; }

	T* CreateNew()
	{
		if (!IsFull())
		{
			T* newItem = AllocUninitialisedItem();
			return new(newItem) T();
		}
		else
		{
			return nullptr;
		}
	}

	template <typename... ConstructorArgs>
	T* CreateNew(ConstructorArgs... args)
	{
		if (!IsFull())
		{
			T* newItem = AllocUninitialisedItem();
			return new(newItem) T(args ...);
		}
		else
		{
			return nullptr;
		}
	}

	T* Create(const T& item)
	{
		if (!IsFull())
		{
			T* newItem = AllocUninitialisedItem();
			return new(newItem) T(item);
		}
		else
		{
			return nullptr;
		}
	}

	void Free(T* item)
	{
		PoolObject* poolItem = reinterpret_cast<PoolObject*>(item);
		poolItem->object.~T();
		poolItem->next = m_nextFree;
		poolItem->active = false;
		m_nextFree = poolItem;
	}

	bool Contains(T* item) const
	{
		int index = int(item - &(*begin()));
		return index >= 0 && index < SIZE;
	}
};
