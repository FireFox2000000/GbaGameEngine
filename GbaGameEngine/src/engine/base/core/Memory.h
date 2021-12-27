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


inline static void MemCopy(const void* src, void* dest, u32 size)
{
	std::memcpy(dest, src, size);
}

inline static void VramSafeMemCopy(const void* src, void* dest, u32 size)
{
	tonccpy(dest, src, size);
}

/// <summary>
/// VRAM-safe memset, byte version
/// </summary>
/// <param name="dest"></param>
/// <param name="val"></param>
/// <param name="size">Size in bytes.</param>
inline static void VramSafeMemSet(void* dest, const u8 val, u32 size)
{
	toncset(dest, val, size);
}

/// <summary>
/// VRAM-safe memset, halfword version
/// </summary>
/// <param name="dest"></param>
/// <param name="val"></param>
/// <param name="size">Size in hwords.</param>
inline static void VramSafeMemSet(void* dest, const u16 val, u32 size)
{
	toncset16(dest, val, size);
}

/// <summary>
/// VRAM-safe memset, word version
/// </summary>
/// <param name="dest"></param>
/// <param name="val"></param>
/// <param name="size">Size in words.</param>
inline static void VramSafeMemSet(void* dest, const u32 val, u32 size)
{
	toncset32(dest, val, size);
}

inline static void SafeFree(void* ptr)
{
	if (ptr != NULL) std::free(ptr);
}

inline static void MoveMemory(void * destination, const void * source, u32 size)
{
	std::memmove(destination, source, size);
}
