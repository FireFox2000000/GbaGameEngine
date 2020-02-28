#pragma once
#include "SharedPtr.h"
#include "Array.h"

// A class that's used to pre-allocate a collection of shared pointer data. Faster allocations and avoids fragmentation when shared pointers are created and destroyed in quick succession.
template <typename T, int CAPACITY>
class SharedPtrPool {
	using tSharedPtrObj = _SharedPtr::SharedPtrObj<T>;

public:
	struct PooledDelete
	{
		void operator()(_SharedPtr::SharedPtrObj<T>* sharedPtrObj) const {
			sharedPtrObj->Get().~T();
			*(sharedPtrObj) = _SharedPtr::SharedPtrObj<T>::Null;	// Marks the original object as null
		}
	};

	class SharedPtr : public _SharedPtr::SharedPtr_Base<T, PooledDelete>
	{
	public:
		SharedPtr() {};
	private:

		SharedPtr(tSharedPtrObj* obj) : _SharedPtr::SharedPtr_Base<T, PooledDelete>(obj) {}

	public:

		template <typename... ConstructorArgs>
		static SharedPtr MakeShared(tSharedPtrObj* staticObj, ConstructorArgs... args)
		{
			// Todo, probably wanna assert staticObj->IsNull();
			tSharedPtrObj* data = new(staticObj) tSharedPtrObj(0, args...);
			data->IncRef();
			return SharedPtr(data);
		}
	};

private:
	Array<tSharedPtrObj, CAPACITY> m_ptrs;
	int m_nextFree = 0;

	tSharedPtrObj* GetNextFree()
	{
		int startIndex = m_nextFree;

		for (; m_nextFree < (int)m_ptrs.Count(); ++m_nextFree)
		{
			if (m_ptrs[m_nextFree].IsNull())
			{
				return &m_ptrs[m_nextFree++];
			}
		}

		// Wrap around back to the start
		for (m_nextFree = 0; m_nextFree < startIndex; ++m_nextFree)
		{
			if (m_ptrs[m_nextFree].IsNull())
			{
				return &m_ptrs[m_nextFree++];
			}
		}

		return nullptr;
	}

public:
	template <typename... ConstructorArgs>
	SharedPtr MakeShared(ConstructorArgs... args)
	{
		auto * freePtr = GetNextFree();

		if (freePtr)
			return SharedPtr::MakeShared(freePtr, args...);
		else
			return SharedPtr();
	}
};
