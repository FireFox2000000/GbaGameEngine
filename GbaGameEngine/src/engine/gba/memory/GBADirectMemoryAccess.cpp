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

static Register& GetRegister(GBA::DirectMemoryAccess::Channels channel)
{
	return (*reinterpret_cast<RegisterList*>(REG_DMA))[channel];
}

void GBA::DirectMemoryAccess::Transfer(Channels channel, volatile void * dst, const void * src, u32 count, u32 mode)
{
	Reset(channel);

	Register& r = GetRegister(channel);
	r.dst = dst;
	r.src = src;
	r.control = count | mode;
}

void GBA::DirectMemoryAccess::Reset(Channels channel)
{
	Register& r = GetRegister(channel);
	r.control = 0;
}
