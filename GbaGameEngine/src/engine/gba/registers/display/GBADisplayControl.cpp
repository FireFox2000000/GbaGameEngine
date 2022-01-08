#include "GBADisplayControl.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"

// http://www.coranac.com/tonc/text/video.htm#tbl-reg-dispcnt

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

struct DisplayControlRegister
{
	u16 videoMode : 3
		, : 1
		, pageSelect : 1 // Modes 4 and 5 can use page flipping for smoother animation. This bit selects the displayed page (and allowing the other one to be drawn on without artifacts).
		, oamHBlank : 1 // Allows access to OAM in an HBlank. OAM is normally locked in VDraw. Will reduce the amount of sprite pixels rendered per line.
		, spriteMappingMode : 1
		, forceScreenBlank : 1
		, enableBackground0 : 1
		, enableBackground1 : 1
		, enableBackground2 : 1
		, enableBackground3 : 1
		, enableSprites : 1
		, enableWindow0 : 1
		, enableWindow1 : 1
		, enableWindowObject
		;
};

struct DisplayControlRegisterReadOnly
{
	const u16 : 3
		, isGbc : 1		// Has a Game Boy Colour cart been inserted?
		;
};

volatile DisplayControlRegister& displayControlRegister = *reinterpret_cast<DisplayControlRegister*>(REG_DISPCNT);
const volatile DisplayControlRegisterReadOnly& displayControlRegisterReadOnly = *reinterpret_cast<const volatile DisplayControlRegisterReadOnly*>(REG_DISPCNT);
vu16& s_REG_DISPCNT = *reinterpret_cast<vu16*>(REG_DISPCNT);

// Default screen res
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

Vector2<int> GBA::DisplayControl::GetScreenResolution()
{
	return m_screenResolution;
}

GBA::DisplayOptions::VideoMode GBA::DisplayControl::GetVideoMode()
{
	return DisplayOptions::VideoMode(displayControlRegister.videoMode);
}

GBA::DisplayOptions::SpriteMappingMode GBA::DisplayControl::GetSpriteMappingMode()
{
	return DisplayOptions::SpriteMappingMode(displayControlRegister.spriteMappingMode);
}

bool GBA::DisplayControl::TestObjectRendering(DisplayOptions::ObjectRendering val)
{
	return (s_REG_DISPCNT & val) != 0;
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
