#pragma once

#include <new>
#include <cstring>
#include <cstdlib>
#include <malloc.h>
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "external/tonc/memory/tonccpy.h"
#include "external/tonc/memory/toncset.h"

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


inline static void MemCopy(void* dest, const void* src, u32 size)
{
	std::memcpy(dest, src, size);
}

template <typename T>
inline static void VramSafeMemCopy(T* dest, const T* src, u32 size)
{
	tonccpy(dest, src, sizeof(T) * size);
}

template <typename T>
inline static void VramSafeMemSet(T* dest, const T val, u32 size)
{
	if constexpr (sizeof(val) == sizeof(u32))
	{
		toncset32(dest, *reinterpret_cast<const u32*>(&val), size);
	}
	else if constexpr (sizeof(val) == sizeof(u16))
	{
		toncset16(dest, *reinterpret_cast<const u16*>(&val), size);
	}
	else if constexpr (sizeof(val) == sizeof(u8))
	{
		toncset(dest, *reinterpret_cast<const u8*>(&val), size);
	}
	else
	{
		static_assert(sizeof(val) < 0, "VramSafeMemSet size not valid");
	}
}

inline static void SafeFree(void* ptr)
{
	if (ptr != NULL) std::free(ptr);
}

inline static void MoveMemory(void * dest, const void * src, u32 size)
{
	std::memmove(dest, src, size);
}
