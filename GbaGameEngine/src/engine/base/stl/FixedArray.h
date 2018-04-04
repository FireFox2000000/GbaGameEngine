#pragma once
#include "../Typedefs.h"
#include "../Macros.h"

template<class T, u32 size>
class FixedArray
{
	T buffer[size];

public:
	FixedArray();
	FixedArray(const FixedArray<T, size> & that);
	~FixedArray();

	void operator = (const FixedArray<T, size>& that) volatile
	{
		for (u32 i = 0; i < size; ++i)
		{
			buffer[i] = that.buffer[i];
		}
	}

	inline T & operator[](u32 index);
	inline T * At(u32 index);

	inline const T & operator[](u32 index) const;
	inline const T * At(u32 index) const;
	inline u32 Length() const;

	inline volatile T & operator[](u32 index) volatile;
	inline volatile T * At(u32 index) volatile;
	inline u32 Length() volatile;
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
inline FixedArray<T, size>::FixedArray(const FixedArray<T, size>& that)
{
	*this = that;
}

template<class T, u32 size>
FixedArray<T, size>::~FixedArray()
{
}

template<class T, u32 size>
inline T& FixedArray<T, size>::operator[](u32 index) {
	return buffer[index];
}

template<class T, u32 size>
inline volatile T& FixedArray<T, size>::operator[](u32 index) volatile {
	return buffer[index];
}

template<class T, u32 size>
inline const T& FixedArray<T, size>::operator[](u32 index) const {
	return buffer[index];
}

template<class T, u32 size>
inline T* FixedArray<T, size>::At(u32 index) {
	return index < Length() ? &buffer[index] : NULL;
}

template<class T, u32 size>
inline volatile T* FixedArray<T, size>::At(u32 index) volatile {
	return index < Length() ? &buffer[index] : NULL;
}

template<class T, u32 size>
inline const T* FixedArray<T, size>::At(u32 index) const {
	return index < Length() ? &buffer[index] : NULL;
}

template<class T, u32 size>
inline u32 FixedArray<T, size>::Length() const {
	return size;
}

template<class T, u32 size>
inline u32 FixedArray<T, size>::Length() volatile {
	return size;
}
