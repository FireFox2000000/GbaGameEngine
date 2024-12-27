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
			PoolObject* m_next;
			T m_object;
		};

		// Basically an "in use" flag to determine if "object" is live or not to figure out whether to call destrutor on object for an auto-free
		bool m_active = false;	// This may be better in SOA alignment for iterator speed when pools are mostly empty. 
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

public:
	FixedPool()
	{
		// To create the free list, we first initialize the head of the list, setting its value to the starting address of the pool�fs memory block.
		// Next, using the element�fs size in bytes and the number of elements in the pool, we calculate the address of the last element.
		// Finally, knowing the number of elements and the last element�fs address, we iterate through each element in the pool, 
		// writing the address of the next element in its memory block, with the exception of the last element, whose next pointer value is set to null.

		auto* objects = reinterpret_cast<PoolObject*>(&m_objects[0]);
		for (int i = 0; i < SIZE; ++i)
		{
			auto* current = objects + i;
			int next = i + 1;
			if (next >= SIZE)
			{
				current->m_next = nullptr;
			}
			else
			{
				current->m_next = objects + next;
			}

			current->m_active = false;
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
			if (current->m_active)
			{
				Free(&current->m_object);
				++freeCount;
			}
		}

		DEBUG_LOGFORMAT("Pool automatically freed %d objects that were still in use.", freeCount);

		m_nextFree = nullptr;
	}

	// Non-copyable as any allocated objects would be duplicated with completely different memory addresses
	FixedPool(const FixedPool&) = delete;
	FixedPool& operator=(const FixedPool&) = delete;

private:
	T* AllocUninitialisedItem()
	{
		// Retrieve the next free item
		m_nextFree->m_active = true;
		T* newItem = &m_nextFree->m_object;

		// Update the next free item in the list
		auto* nextFree = m_nextFree->m_next;
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

	inline PoolObject* ObjectFromItem(T* item) const
	{
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
		static_assert(offsetof(typename IPool<T>::PoolObject, m_object) == 0, "Convertion from Pool item T to PoolObject will fail.");
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
		return reinterpret_cast<PoolObject*>(item);
	}

	inline const PoolObject* ObjectFromItem(const T* item) const
	{
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
		static_assert(offsetof(typename IPool<T>::PoolObject, m_object) == 0, "Convertion from Pool item T to PoolObject will fail.");
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
		return reinterpret_cast<const PoolObject*>(item);
	}

	inline int IndexOf(const PoolObject* item) const
	{
		return static_cast<int>(item - GetAt(0));
	}

	inline int IndexOf(const T* item) const
	{
		const PoolObject* poolItem = ObjectFromItem(item);
		return IndexOf(poolItem);
	}

public:
	constexpr static uint Capacity() { return SIZE; }

	class iterator_base
	{
	protected:
		PoolObject* m_current = nullptr;
		PoolObject* m_end = nullptr;

		void Advance()
		{
			DEBUG_ASSERTMSG(m_current < m_end, "Advancing past the end iterator, this will result in infinite loop");

			do
			{
				++m_current;
			} while (m_current < m_end && !m_current->m_active);
		}

		T& Get()
		{
			return m_current->m_object;
		}

		const T& Get() const
		{
			return m_current->m_object;
		}

	public:
		iterator_base(PoolObject* current, PoolObject* end) : m_current(current), m_end(end)
		{
			DEBUG_ASSERTMSG(m_current <= m_end, "Pool current iterator is greater than end, this will result in infinite loop");
			if (m_current < m_end && !m_current->m_active)
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

		const_iterator& operator ++ () 
		{
			iterator_base::Advance();
			return *this;
		}
	};

	iterator begin() { return iterator(GetAt(0), GetAt(SIZE)); }
	iterator end() { return iterator(GetAt(SIZE), GetAt(SIZE)); }

	const_iterator begin() const { return const_iterator(const_cast<PoolObject*>(GetAt(0)), const_cast<PoolObject*>(GetAt(SIZE)));}
	const_iterator end() const { return const_iterator(const_cast<PoolObject*>(GetAt(SIZE)), const_cast<PoolObject*>(GetAt(SIZE))); }

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
		DEBUG_ASSERTMSG(Contains(item), "Incorrectly freeing item that was not originally allocated from the pool");

		PoolObject* poolItem = ObjectFromItem(item);
		poolItem->m_object.~T();
		poolItem->m_next = m_nextFree;
		poolItem->m_active = false;
		m_nextFree = poolItem;
	}

	bool Contains(T* item) const
	{
		const PoolObject* poolItem = ObjectFromItem(item);
		int index = int(poolItem - GetAt(0));
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
	using Block = FixedPool<T, BLOCK_SIZE>;
	using BlocksList = List<Block*>;

	BlocksList m_blocks;

	Block* GetOrCreateNextAllocatableBlock()
	{
		for (auto* block : m_blocks)
		{
			if (!block->IsFull())
			{
				return block;
			}
		}

		// All blocks are full, allocate a new block
		auto* newBlock = new Block;
		m_blocks.Add(newBlock);

		return newBlock;
	}

public:
	Pool() = default;

	~Pool()
	{
		for (auto* block : m_blocks)
		{
			delete block;
		}
	}

	// Non-copyable as any allocated objects would be duplicated with completely different memory addresses
	Pool(const Pool&) = delete;
	Pool& operator=(const Pool&) = delete;

	// TODO move constructor as we can move the pool while keeping the blocks in the same memory locations
	// and thus preserving memory addresses

	uint Capacity() const { return m_blocks.Count() * Block::Capacity(); }

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
				return;
			}
		}

		DEBUG_ASSERTMSGFORMAT(false, "Failed to free item at %p, not allocated from this pool %p.", item, this);
	}

	class iterator_base
	{
	protected:
		BlocksList::iterator m_currentBlock;
		Block::iterator m_current;
		const typename Block::iterator m_end;
		Block::iterator m_currentBlockEnd;

		inline void AdvanceToNextValidBlock()
		{
			do
			{
				++m_currentBlock;
				m_current = (*m_currentBlock)->begin();
				m_currentBlockEnd = (*m_currentBlock)->end();
			} while (m_current != m_end && m_current == m_currentBlockEnd);
		}

		void Advance()
		{
			++m_current;

			if (m_current == m_currentBlockEnd && m_current != m_end)
			{
				AdvanceToNextValidBlock();
			}
		}

		T& Get()
		{
			return *m_current;
		}

		const T& Get() const
		{	
			return **const_cast<Block::iterator*>(&m_current);
		}

	public:
		iterator_base(
			BlocksList::iterator currentBlock
			, Block::iterator current
			, Block::iterator end)
			: m_currentBlock(currentBlock)
			, m_current(current)
			, m_end(end)
			, m_currentBlockEnd(m_currentBlock ? (*m_currentBlock)->end() : end)
		{
			if (m_currentBlock && m_current != m_end && m_current == m_currentBlockEnd)
			{
				AdvanceToNextValidBlock();
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
		iterator(
			BlocksList::iterator currentBlock
			, Block::iterator current
			, Block::iterator end)
			: iterator_base(currentBlock, current, end)
		{}

		T& operator * () { return  iterator_base::Get(); }
		T* operator -> () { return &iterator_base::Get(); }

		iterator& operator ++ () {
			iterator_base::Advance();
			return *this;
		}
	};

	class const_iterator : public iterator_base
	{
	public:
		const_iterator(
			BlocksList::iterator currentBlock
			, Block::iterator current
			, Block::iterator end)
			: iterator_base(currentBlock, current, end)
		{}

		const T& operator * () const { return  iterator_base::Get(); }
		const T* operator -> () const { return &iterator_base::Get(); }

		const_iterator& operator ++ () 
		{
			iterator_base::Advance();
			return *this;
		}
	};

	iterator begin() 
	{ 
		return iterator(
			m_blocks.begin(), m_blocks.begin() != m_blocks.end() ? (*m_blocks.begin())->begin() : typename Block::iterator(nullptr, nullptr),
			m_blocks.begin() != m_blocks.end() ? (m_blocks[m_blocks.Count() - 1])->end() : typename Block::iterator(nullptr, nullptr)
		);
	}

	iterator end()
	{
		return iterator(
			m_blocks.end(), m_blocks.begin() != m_blocks.end() ? (m_blocks[m_blocks.Count() - 1])->end() : typename Block::iterator(nullptr, nullptr),
			m_blocks.begin() != m_blocks.end() ? (m_blocks[m_blocks.Count() - 1])->end() : typename Block::iterator(nullptr, nullptr)
		);
	}
	
	const_iterator begin() const 
	{
		BlocksList& mutableBlocks = *const_cast<BlocksList*>(&m_blocks);
		return const_iterator(
			mutableBlocks.begin(), mutableBlocks.begin() != mutableBlocks.end() ? (*mutableBlocks.begin())->begin() : typename Block::iterator(nullptr, nullptr),
			mutableBlocks.begin() != mutableBlocks.end() ? (mutableBlocks[mutableBlocks.Count() - 1])->end() : typename Block::iterator(nullptr, nullptr)
		);
	}

	const_iterator end() const 
	{
		BlocksList& mutableBlocks = *const_cast<BlocksList*>(&m_blocks);
		return const_iterator(
			mutableBlocks.end(), mutableBlocks.begin() != mutableBlocks.end() ? (mutableBlocks[mutableBlocks.Count() - 1])->end() : typename Block::iterator(nullptr, nullptr),
			mutableBlocks.begin() != mutableBlocks.end() ? (mutableBlocks[mutableBlocks.Count() - 1])->end() : typename Block::iterator(nullptr, nullptr)
		);
	}
};
