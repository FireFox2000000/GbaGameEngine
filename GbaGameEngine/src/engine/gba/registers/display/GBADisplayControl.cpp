#include "GBADisplayControl.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"

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

void GBA::DisplayControl::SetBackgroundActive(int backgroundIndex, bool active)
{
	if (backgroundIndex < GBA::BackgroundControl::Backgrounds::Count)
	{
		u32 backgroundBit = DisplayOptions::Background0 << backgroundIndex;
		if (active)
		{
			s_REG_DISPCNT |= backgroundBit;
		}
		else
		{
			s_REG_DISPCNT &= ~backgroundBit;
		}
	}
}

Bitmask<u8> GBA::DisplayControl::GetBackgroundsForCurrentVideoMode()
{
	Bitmask<u8> mask;

	switch (GetVideoMode())
	{
	case DisplayOptions::VideoMode::Mode0:
	{
		mask.SetBit(0);
		mask.SetBit(1);
		mask.SetBit(2);
		mask.SetBit(3);
		break;
	}
	case DisplayOptions::VideoMode::Mode1:
	{
		mask.SetBit(0);
		mask.SetBit(1);
		mask.SetBit(2);
		break;
	}
	case DisplayOptions::VideoMode::Mode2:
	{
		mask.SetBit(2);
		mask.SetBit(3);
		break;
	}
	default:
		break;
	}

	return mask;
}
