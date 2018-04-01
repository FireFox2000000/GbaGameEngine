#pragma once
#include "../Typedefs.h"

template<class T, u32 size>
class FixedArray
{
	T buffer[size];

public:
	FixedArray();
	~FixedArray();

	T & operator[](u32 index);
	const T & operator[](u32 index) const;

	u32 Length() const;
};

template<class T, u32 size>
FixedArray<T, size>::FixedArray()
{
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
u32 FixedArray<T, size>::Length() const {
	return size;
}
