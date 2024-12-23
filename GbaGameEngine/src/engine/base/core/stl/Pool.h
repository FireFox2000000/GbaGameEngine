// https://www.gamasutra.com/blogs/MateusGondimLima/20180720/322503/Designing_and_implementing_a_pool_allocator_data_structure_for_memory_management_in_games.php
// Basically an array, but unused elements store pointer addresses in a linked-list fashion to the rest of the free elements
// Has automatic cleanup of currently active objects that have no been free upon destruction. 

// ALso probably worth looking at https://gameprogrammingpatterns.com/object-pool.html as well

#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/stl/List.h"

template<class T>
class IPool
{
protected:
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

public:
	virtual ~IPool() = default;

	virtual T* CreateNew() = 0;
	virtual T* Create(const T& item) = 0;
	virtual void Free(T* item) = 0;
};

template<class T, int SIZE>
class FixedPool : public IPool<T>
{
	using PoolObject = IPool<T>::PoolObject;

	// Create a u8 array instead of PoolObject array to get around union implicitly deleting destructor error
	// We'll handle the destructor calls ourselves. 
	alignas(PoolObject) u8 m_objects[sizeof(PoolObject) * SIZE];
	PoolObject* m_nextFree = nullptr;

	T* AllocUninitialisedItem()
	{
		// Retrieve the next free item
		m_nextFree->active = true;
		T* newItem = &m_nextFree->object;

		// Update the next free item in the list
		auto* nextFree = m_nextFree->next;
		m_nextFree = nextFree;

		return newItem;
	}

	PoolObject* GetAt(int index)
	{
		auto* objects = reinterpret_cast<PoolObject*>(&m_objects[0]);
		return objects + index;
	}

	const PoolObject* GetAt(int index) const
	{
		const auto* objects = reinterpret_cast<const PoolObject*>(&m_objects[0]);
		return objects + index;
	}

public:
	FixedPool()
	{
		// To create the free list, we first initialize the head of the list, setting its value to the starting address of the poolÅfs memory block.
		// Next, using the elementÅfs size in bytes and the number of elements in the pool, we calculate the address of the last element.
		// Finally, knowing the number of elements and the last elementÅfs address, we iterate through each element in the pool, 
		// writing the address of the next element in its memory block, with the exception of the last element, whose next pointer value is set to null.

		auto* objects = reinterpret_cast<PoolObject*>(&m_objects[0]);
		for (int i = 0; i < SIZE; ++i)
		{
			auto* current = objects + i;
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

	~FixedPool()
	{
		// Safe free all
		int freeCount = 0;

		auto* objects = reinterpret_cast<PoolObject*>(&m_objects[0]);
		for (int i = 0; i < SIZE; ++i)
		{
			auto* current = objects + i;
			if (current->active)
			{
				Free(&current->object);
				++freeCount;
			}
		}

		DEBUG_LOGFORMAT("Pool automatically freed %d objects that were still in use.", freeCount);

		m_nextFree = nullptr;
	}

	class iterator_base
	{
	protected:
		PoolObject* m_current = nullptr;
		PoolObject* m_end = nullptr;

		void Advance()
		{
			do
			{
				++m_current;
			} while (m_current < m_end && !m_current->active);
		}

		T& Get()
		{
			return m_current->object;
		}

		const T& Get() const
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

		T& operator * () { return iterator_base::Get(); }
		T* operator -> () { return &iterator_base::Get(); }

		iterator& operator ++ () {
			iterator_base::Advance();
			return *this;
		}
	};

	class const_iterator : public iterator_base
	{
	public:
		const_iterator(PoolObject* current, PoolObject* end) : iterator_base(current, end) {}

		const T& operator * () const { return iterator_base::Get(); }
		const T* operator -> () const { return &iterator_base::Get(); }

		const_iterator& operator ++ () {
			iterator_base::Advance();
			return *this;
		}
	};

	iterator begin() { return iterator(GetAt(0), GetAt(SIZE)); }
	const_iterator begin() const {
		return const_iterator(const_cast<PoolObject*>(GetAt(0)), const_cast<PoolObject*>(GetAt(SIZE)));
	}

	iterator end() { return iterator(GetAt(SIZE), GetAt(SIZE)); }
	const_iterator end() const {
		return const_iterator(const_cast<PoolObject*>(GetAt(SIZE)), const_cast<PoolObject*>(GetAt(SIZE)));
	}

	inline bool IsFull() { return !m_nextFree; }

	T* CreateNew() override
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
	T* CreateNew(ConstructorArgs&&... args)
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

	T* Create(const T& item) override
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

	void Free(T* item) override
	{
		auto* poolItem = reinterpret_cast<PoolObject*>(item);
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

/// <summary>
/// Dynamic allocation pool of objects. Allocates in blocks of objects, so that if one block is filled up,
/// we just allocate another block and start pooling objects from there
/// </summary>
template<class T, int BLOCK_SIZE>
class Pool : public IPool<T>
{
	using PoolObject = IPool<T>::PoolObject;

	List<FixedPool<T, BLOCK_SIZE>*> m_blocks;

	FixedPool<T, BLOCK_SIZE>* GetOrCreateNextAllocatableBlock()
	{
		for (auto* block : m_blocks)
		{
			if (!block->IsFull())
			{
				return block;
			}
		}

		// All blocks are full, allocate a new block
		auto* newBlock = new FixedPool<T, BLOCK_SIZE>;
		m_blocks.Add(newBlock);

		return newBlock;
	}

public:
	~Pool()
	{
		for (auto* block : m_blocks)
		{
			delete block;
		}
	}

	T* CreateNew() override
	{
		return GetOrCreateNextAllocatableBlock()->CreateNew();
	}

	template <typename... ConstructorArgs>
	T* CreateNew(ConstructorArgs&&... args)
	{
		GetOrCreateNextAllocatableBlock()->CreateNew(std::forward<ConstructorArgs>(args)...);
	}

	T* Create(const T& item) override
	{
		return GetOrCreateNextAllocatableBlock()->Create(item);
	}

	void Free(T* item) override
	{
		for (auto* block : m_blocks)
		{
			if (block->Contains(item))
			{
				block->Free(item);
			}
		}
	}

	class iterator_base
	{
	protected:
		List<FixedPool<T, BLOCK_SIZE>*>::iterator m_currentBlock;
		FixedPool<T, BLOCK_SIZE>::iterator m_current;
		const typename List<FixedPool<T, BLOCK_SIZE>*>::iterator m_endBlock;
		const typename FixedPool<T, BLOCK_SIZE>::iterator m_end;

		void Advance()
		{
			++m_current;

			if (m_current == (*m_currentBlock)->end())
			{
				++m_currentBlock;

				if (m_current != m_end)
				{
					m_current = (*m_currentBlock)->begin();
				}
			}
		}

		T& Get()
		{
			return *m_current;
		}

		const T& Get() const
		{
			return *m_current;
		}

	public:
		iterator_base(
			List<FixedPool<T, BLOCK_SIZE>*>::iterator currentBlock
			, FixedPool<T, BLOCK_SIZE>::iterator current
			, List<FixedPool<T, BLOCK_SIZE>*>::iterator endBlock
			, FixedPool<T, BLOCK_SIZE>::iterator end)
			: m_currentBlock(currentBlock)
			, m_current(current)
			, m_endBlock(endBlock)
			, m_end(end)
		{
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
		iterator(
			List<FixedPool<T, BLOCK_SIZE>*>::iterator currentBlock
			, FixedPool<T, BLOCK_SIZE>::iterator current
			, List<FixedPool<T, BLOCK_SIZE>*>::iterator endBlock
			, FixedPool<T, BLOCK_SIZE>::iterator end)
			: iterator_base(currentBlock, current, endBlock, end) 
		{}

		T& operator * () { return iterator_base::Get(); }
		T* operator -> () { return &iterator_base::Get(); }

		iterator& operator ++ () {
			iterator_base::Advance();
			return *this;
		}
	};

	class const_iterator : public iterator_base
	{
	public:
		const_iterator(List<FixedPool<T, BLOCK_SIZE>*>::iterator currentBlock
			, FixedPool<T, BLOCK_SIZE>::iterator current
			, List<FixedPool<T, BLOCK_SIZE>*>::iterator endBlock
			, FixedPool<T, BLOCK_SIZE>::iterator end)
			: iterator_base(currentBlock, current, endBlock, end)
		{}

		const T& operator * () const { return iterator_base::Get(); }
		const T* operator -> () const { return &iterator_base::Get(); }

		const_iterator& operator ++ () {
			iterator_base::Advance();
			return *this;
		}
	};

	iterator begin() 
	{ 
		return iterator(
			m_blocks.begin(), m_blocks.begin() != m_blocks.end() ? (*m_blocks.begin())->begin() : typename FixedPool<T, BLOCK_SIZE>::iterator(nullptr, nullptr),
			m_blocks.end(), m_blocks.begin() != m_blocks.end() ? (m_blocks[m_blocks.Count() - 1])->end() : typename FixedPool<T, BLOCK_SIZE>::iterator(nullptr, nullptr)
		);
	}

	iterator end()
	{
		return iterator(
			m_blocks.end(), m_blocks.begin() != m_blocks.end() ? (m_blocks[m_blocks.Count() - 1])->end() : typename FixedPool<T, BLOCK_SIZE>::iterator(nullptr, nullptr),
			m_blocks.end(), m_blocks.begin() != m_blocks.end() ? (m_blocks[m_blocks.Count() - 1])->end() : typename FixedPool<T, BLOCK_SIZE>::iterator(nullptr, nullptr)
		);
	}

	/*
	const_iterator begin() const 
	{
		return const_iterator(m_blocks[0], m_blocks[0]->begin(), m_blocks + BLOCK_SIZE, (m_blocks + BLOCK_SIZE - 1)->end());
	}

	

	const_iterator end() const 
	{
		return const_iterator(m_blocks + BLOCK_SIZE, (m_blocks + BLOCK_SIZE - 1)->end(), m_blocks + BLOCK_SIZE, (m_blocks + BLOCK_SIZE - 1)->end());
	}*/
};
