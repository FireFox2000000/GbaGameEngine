#include "GBADisplayStatus.h"
#include "engine/gba/registers/RegisterMap.h"

vu32& s_REG_DISPSTAT = (*(vu32*)(REG_DISPSTAT));
const vu16& s_REG_VCOUNT = (*(vu16*)(REG_VCOUNT));

enum Flags
{
	VBlankStatus = BIT(0),
	HBlankStatus = BIT(1),
	VCountTriggerStatus = BIT(2),	// Set if the current scanline matches the scanline trigger ( REG_VCOUNT == REG_DISPSTAT{8-F} )
	VBlankInterruptRequest = BIT(3),
	HBlankInterruptRequest = BIT(4),
	VCountInterruptRequest = BIT(15), // Fires interrupt if current scanline matches trigger value.
};

bool GBA::DisplayStatus::IsStatusBitSet(int bitIndex)
{
	return s_REG_DISPSTAT & bitIndex;
}

bool GBA::DisplayStatus::InVBlank()
{
	return IsStatusBitSet(VBlankStatus);
}

bool GBA::DisplayStatus::InHBlank()
{
	return IsStatusBitSet(HBlankStatus);
}

vu16 GBA::DisplayStatus::VCount()
{
	return s_REG_VCOUNT;
}
