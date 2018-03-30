#include "GBADisplayControl.h"
#include "../../base/Typedefs.h"
#include "../memory/GBAMemoryLocations.h"

// http://www.coranac.com/tonc/text/video.htm#tbl-reg-dispcnt

#define REG_DISPCNT (*(vu32*)IO_REGISTERS)
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

void GBA::DisplayControl::SetDisplayOptions(int params)
{
	REG_DISPCNT = params;
}

GBA::DisplayOptions::VideoMode GBA::DisplayControl::GetVideoMode()
{
	// Video mode is the first 3 bits
	return DisplayOptions::VideoMode(REG_DISPCNT & SHIFTED_BITMASK(0x7, 0));
}

Vector2 GBA::DisplayControl::GetScreenResolution()
{
	using namespace DisplayOptions;

	VideoMode mode = GetVideoMode();

	switch (mode)
	{
	case (Mode5):
		return Vector2(160, 128);
		break;
	default:
		break;
	}

	return Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
}
