#pragma once
#include "string.h"
#include "../Typedefs.h"

inline static void MemCopy(const void* src, void* dest, u32 size)
{
	memcpy(dest, src, size);
}
