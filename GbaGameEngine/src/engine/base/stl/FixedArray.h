#pragma once
#include "../Typedefs.h"
#include "../Macros.h"

template<class T, u32 size>
class FixedArray
{
	T buffer[size];

public:
	FixedArray();
	~FixedArray();

	T & operator[](u32 index);
	const T & operator[](u32 index) const;

	T * At(u32 index);
	const T * At(u32 index) const;
	u32 Length() const;
};

template<class T, u32 size>
FixedArray<T, size>::FixedArray()
{
	// Default initialisation. Do it the hard way cause we don't have C++11. Woo.
	for (u32 i = 0; i < size; ++i)
	{
		buffer[i] = T();
	}
}

template<class T, u32 size>
FixedArray<T, size>::~FixedArray()
{
}

template<class T, u32 size>
T& FixedArray<T, size>::operator[](u32 index) {
	return buffer[index];
}

template<class T, u32 size>
const T& FixedArray<T, size>::operator[](u32 index) const {
	return buffer[index];
}

template<class T, u32 size>
T* FixedArray<T, size>::At(u32 index) {
	return index < Length() ? &buffer[index] : NULL;
}

template<class T, u32 size>
const T* FixedArray<T, size>::At(u32 index) const {
	return index < Length() ? &buffer[index] : NULL;
}

template<class T, u32 size>
u32 FixedArray<T, size>::Length() const {
	return size;
}
