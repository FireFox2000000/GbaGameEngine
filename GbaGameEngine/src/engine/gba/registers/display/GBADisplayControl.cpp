#include "GBADisplayControl.h"
#include "engine/gba/registers/RegisterMap.h"

// http://www.coranac.com/tonc/text/video.htm#tbl-reg-dispcnt

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

vu32& GBA::DisplayControl::s_REG_DISPCNT = (*(vu32*)REG_DISPCNT);
Vector2<int> GBA::DisplayControl::m_screenResolution = Vector2<int>(SCREEN_WIDTH, SCREEN_HEIGHT);

void GBA::DisplayControl::SetDisplayOptions(int params)
{
	s_REG_DISPCNT = params;

	{
		using namespace DisplayOptions;

		VideoMode mode = GetVideoMode();

		switch (mode)
		{
		case (Mode5):
			m_screenResolution = Vector2<int>(160, 128);
			break;
		default:
			m_screenResolution = Vector2<int>(SCREEN_WIDTH, SCREEN_HEIGHT);
			break;
		}
	}
}
