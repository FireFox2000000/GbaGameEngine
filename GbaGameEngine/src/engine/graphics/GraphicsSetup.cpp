#include "GraphicsSetup.h"
#include "gbatek/DisplayControl.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"

void GraphicsSetup::InitialiseStandardGraphics()
{
	using namespace GBA::Gfx;

	// We don't need to enable background manually here. That happens during tilemap rendering automatically.
	GBATEK::ioRegisterDisplayControl->forceScreenBlank = false;
	GBATEK::ioRegisterDisplayControl->videoMode = GBATEK::VideoMode::Mode0;
	GBATEK::ioRegisterDisplayControl->enableSprites = true;
	GBATEK::ioRegisterDisplayControl->objectMappingMode = GBATEK::ObjectMappingMode::OneDimensional;

	{
		auto& palette = *PaletteBank::EditBackgroundPalette();
		constexpr GBATEK::ColourRGB16 alpha = { .r = 0, .g = 0, .b = 0 };
		VramSafeMemSet(&palette[0], alpha, 1);			// When no background are active the gba's default is to render the colour in this slot as the background
	}
	//{
	//	auto& palette = *GBA::Gfx::PaletteBank::EditSpritePalette();
	//	VramSafeMemSet((void*)&palette[0], (u16)0, 1);
	//}
}
