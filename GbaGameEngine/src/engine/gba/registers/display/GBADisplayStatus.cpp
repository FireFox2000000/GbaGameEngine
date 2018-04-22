#include "GBADisplayStatus.h"
#include "engine/gba/registers/RegisterMap.h"

vu32& GBA::DisplayStatus::s_REG_DISPSTAT = (*(vu32*)(REG_DISPSTAT));
const vu16& GBA::DisplayStatus::s_REG_VCOUNT = (*(vu16*)(REG_VCOUNT));
