#include "GBAInput.h"
#include "engine/gba/registers/RegisterMap.h"

bool GBA::Input::GetKey(Buttons::Enum key, const u32 inputChannel)
{
	return !(inputChannel & key);
}

vu32 GBA::Input::GetInputKeys()
{
	return *(vu32*)(REG_KEYINPUT);
}
