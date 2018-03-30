#include "GBADisplayStatus.h"
#include "../../base/Typedefs.h"
#include "../memory/GBAMemoryLocations.h"

#define REG_DISPSTAT (*(vu32*)(IO_REGISTERS + 4))

bool GBA::DisplayStatus::IsBitSet(int bitIndex)
{
	return REG_DISPSTAT & bitIndex;
}
