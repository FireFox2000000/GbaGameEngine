#ifndef PRAGMA_ONCE_ENGINE_BASE_CORE_MEMORY_H
#define PRAGMA_ONCE_ENGINE_BASE_CORE_MEMORY_H

#include "string.h"
#include "../Typedefs.h"

inline static void MemCopy(const void* src, void* dest, u32 size)
{
	memcpy(dest, src, size);
}

#endif
