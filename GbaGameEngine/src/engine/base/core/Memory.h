#ifndef PRAGMA_ONCE_ENGINE_BASE_CORE_MEMORY_H
#define PRAGMA_ONCE_ENGINE_BASE_CORE_MEMORY_H

#include <new>
#include <cstring>
#include <cstdlib>
#include <malloc.h>
#include "../Typedefs.h"
#include "../Macros.h"

template<typename T>
inline static T* MAllocType(u32 count)
{
	return (T*)malloc(sizeof(T) * count);
}

template<typename T>
inline static T* MemAlignedType(u32 alignment, u32 count)
{
	return (T*)memalign(alignment, sizeof(T) * count);
}


inline static void MemCopy(const void* src, void* dest, u32 size)
{
	std::memcpy(dest, src, size);
}

inline static void SafeFree(void* ptr)
{
	if (ptr != NULL) std::free(ptr);
}

inline static void MoveMemory(void * destination, const void * source, u32 size)
{
	std::memmove(destination, source, size);
}

#endif
