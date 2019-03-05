#ifndef PRAGMA_ONCE_ENGINE_BASE_CORE_STL_IARRAY_H
#define PRAGMA_ONCE_ENGINE_BASE_CORE_STL_IARRAY_H

#include "engine/base/Typedefs.h"

template<class T>
class IArray
{
public:
	typedef T* iterator;
	typedef const T* const_iterator;

	virtual ~IArray() = 0;

	virtual u32 Count() = 0;
	virtual u32 Count() const = 0;

	virtual T* begin() = 0;
	virtual const T* begin() const = 0;
	
	virtual T* end() = 0;
	virtual const T* end() const = 0;
	
	virtual T & operator[](u32 index) = 0;
	virtual const T & operator[](u32 index) const = 0;
};

template<class T>
IArray<T>::~IArray() {};

#endif
