// https://www.coranac.com/2008/01/tonccpy/

#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

void __toncset(void *dst, u32 fill, u32 size);
static inline void toncset(void *dst, u8 src, u32 size);
static inline void toncset16(void *dst, u16 src, u32 size);
static inline void toncset32(void *dst, u32 src, u32 size);

static inline u32 quad8(u8 x) { x |= x << 8; return x | x << 16; }

//! VRAM-safe memset, byte version. Size in bytes.
static inline void toncset(void *dst, u8 src, u32 size)
{
	__toncset(dst, quad8(src), size);
}

//! VRAM-safe memset, halfword version. Size in hwords.
static inline void toncset16(void *dst, u16 src, u32 size)
{
	__toncset(dst, src | src << 16, size * 2);
}

//! VRAM-safe memset, word version. Size in words.
static inline void toncset32(void *dst, u32 src, u32 size)
{
	__toncset(dst, src, size * 4);
}
