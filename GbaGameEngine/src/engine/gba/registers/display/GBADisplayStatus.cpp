#include "GBADisplayStatus.h"
#include "engine/gba/registers/RegisterMap.h"

struct DisplayStatusRegister
{
	u16 : 3
	, vBlankInterruptRequestEnabled : 1
	, hBlankInterruptRequestEnabled : 1
	, vCountInterruptRequestEnabled : 1
	, : 2
	, vCountTrigger : 8		// VCount trigger value. If the current scanline is at this value, bit 2 is set and an interrupt is fired if requested.
	;
};

struct DisplayStatusRegisterReadOnly
{
	const u16 isInVBlank : 1
		, isInHBlank : 1
		, isInVCount : 1
	;
};

volatile DisplayStatusRegister& displayStatusRegister = *reinterpret_cast<DisplayStatusRegister*>(REG_DISPSTAT);
const volatile DisplayStatusRegisterReadOnly& displayStatusRegisterReadOnly = *reinterpret_cast<const volatile DisplayStatusRegisterReadOnly*>(REG_DISPSTAT);
const vu16& s_REG_VCOUNT = (*(vu16*)(REG_VCOUNT));

bool GBA::DisplayStatus::InVBlank()
{
	return displayStatusRegisterReadOnly.isInVBlank;
}

bool GBA::DisplayStatus::InHBlank()
{
	return displayStatusRegisterReadOnly.isInHBlank;
}

vu16 GBA::DisplayStatus::VCount()
{
	return s_REG_VCOUNT;
}

void GBA::DisplayStatus::EnableVBlankInterrupts()
{
	displayStatusRegister.vBlankInterruptRequestEnabled = true;
}

void GBA::DisplayStatus::EnableHBlankInterrupts()
{
	displayStatusRegister.hBlankInterruptRequestEnabled = true;

}
