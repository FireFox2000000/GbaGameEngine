#include "GBADirectMemoryAccess.h"
#include "engine/base/core/stl/Array.h"
#include "engine/gba/registers/RegisterMap.h"

struct Register
{
	const void* src;
	volatile void* dst;
	u32 control;
};

using RegisterList = Array<Register, GBA::DirectMemoryAccess::Channels::Count>;

// Return volatile so that control var assignments are in the correct order and aren't optimised out. Weird sneaky effects otherwise. 
static volatile Register& GetRegister(GBA::DirectMemoryAccess::Channels channel)
{
	return (*reinterpret_cast<volatile RegisterList*>(REG_DMA))[channel];
}

void GBA::DirectMemoryAccess::Transfer(Channels channel, volatile void * dst, const void * src, u32 count, u32 mode)
{
	Reset(channel);

	auto& r = GetRegister(channel);

	r.dst = dst;
	r.src = src;
	r.control = count | mode;
}

void GBA::DirectMemoryAccess::Reset(Channels channel)
{
	auto& r = GetRegister(channel);
	r.control = 0;
}
