#include "GBADisplayStatus.h"
#include "../../memory/GBAMemoryLocations.h"

vu32& GBA::DisplayStatus::s_REG_DISPSTAT = (*(vu32*)(IO_REGISTERS + 4));
const vu16& GBA::DisplayStatus::s_REG_VCOUNT = (*(vu16*)(IO_REGISTERS + 6));