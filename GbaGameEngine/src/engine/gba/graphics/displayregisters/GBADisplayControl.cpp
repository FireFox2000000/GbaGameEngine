#include "GBADisplayControl.h"
#include "../../memory/GBAMemoryLocations.h"

// http://www.coranac.com/tonc/text/video.htm#tbl-reg-dispcnt

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

vu32& GBA::DisplayControl::s_REG_DISPCNT = (*(vu32*)IO_REGISTERS);
Vector2 GBA::DisplayControl::m_screenResolution = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);

void GBA::DisplayControl::SetDisplayOptions(int params)
{
	s_REG_DISPCNT = params;

	{
		using namespace DisplayOptions;

		VideoMode mode = GetVideoMode();

		switch (mode)
		{
		case (Mode5):
			m_screenResolution = Vector2(160, 128);
			break;
		default:
			m_screenResolution = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
			break;
		}
	}
}
