#include "GraphicsSetup.h"
#include "GBASDK/DisplayControl.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"

void GraphicsSetup::InitialiseStandardGraphics()
{
	using namespace GBA::Gfx;

	// We don't need to enable background manually here. That happens during tilemap rendering automatically.
	GBA::ioRegisterDisplayControl->forceScreenBlank = false;
	GBA::ioRegisterDisplayControl->videoMode = GBA::VideoMode::Mode0;
	GBA::ioRegisterDisplayControl->enableSprites = true;
	GBA::ioRegisterDisplayControl->objectMappingMode = GBA::ObjectMappingMode::OneDimensional;

	{
		auto& palette = *PaletteBank::EditBackgroundPalette();
		constexpr GBA::ColourRGB16 alpha = { 0, 0, 0 };
		VramSafeMemSet(&palette[0], alpha, 1);			// When no background are active the gba's default is to render the colour in this slot as the background
	}
	//{
	//	auto& palette = *GBA::Gfx::PaletteBank::EditSpritePalette();
	//	VramSafeMemSet((void*)&palette[0], (u16)0, 1);
	//}
}
