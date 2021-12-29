#include "GraphicsSetup.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"

void GraphicsSetup::InitialiseStandardGraphics()
{
	using namespace GBA;
	using namespace GBA::Gfx;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

	{
		auto& palette = *PaletteBank::EditBackgroundPalette();
		VramSafeMemSet((void*)&palette[0], (u16)0, 1);			// When no background are active the gba's default is to render the colour in this slot as the background
	}
	//{
	//	auto& palette = *GBA::Gfx::PaletteBank::EditSpritePalette();
	//	VramSafeMemSet((void*)&palette[0], (u16)0, 1);
	//}
}
