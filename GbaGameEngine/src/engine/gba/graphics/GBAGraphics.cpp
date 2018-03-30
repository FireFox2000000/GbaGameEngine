#include "GBAGraphics.h"
#include "../../base/Typedefs.h"
#include "../memory/GBAMemoryLocations.h"

#define MEM_IO_REG_DISPCNT (*(volatile u32*)IO_REGISTER)

void GBA::Graphics::SetDisplayParameters(int params)
{
	MEM_IO_REG_DISPCNT = params;
}