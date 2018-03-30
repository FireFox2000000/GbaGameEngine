#include "GBAGraphics.h"
#include "../../base/Typedefs.h"
#include "../memory/GBAMemoryLocations.h"

// http://www.coranac.com/tonc/text/video.htm#tbl-reg-dispcnt

#define MEM_IO_REG_DISPCNT (*(volatile u32*)IO_REGISTER)
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

void GBA::Graphics::AddDisplayParameters(int params)
{
	MEM_IO_REG_DISPCNT |= params;
}

GBA::GraphicsParams::VideoMode GBA::Graphics::GetVideoMode()
{
	// Video mode is the first 3 bits
	return GraphicsParams::VideoMode(MEM_IO_REG_DISPCNT & SHIFTED_BITMASK(0x7, 0));
}

Vector2 GBA::Graphics::GetScreenResolution()
{
	using namespace GraphicsParams;

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
