#include "GBADisplayStatus.h"
#include "../memory/GBAMemoryLocations.h"

#define REG_DISPSTAT (*(vu32*)(IO_REGISTERS | 4))
#define REG_VCOUNT (*(vu16*)(IO_REGISTERS | 6))

bool GBA::DisplayStatus::IsStatusBitSet(int bitIndex)
{
	return REG_DISPSTAT & bitIndex;
}

u16 GBA::DisplayStatus::VCount()
{
	return REG_VCOUNT;
}