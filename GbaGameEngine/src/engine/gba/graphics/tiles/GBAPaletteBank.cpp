#include "GBAPaletteBank.h"
#include "../../memory/GBAMemoryLocations.h"
#include "../../../base/Macros.h"

const u8 c_BlockCount4bbp = 16;
const u8 c_ColourCount4bbp = 16;

vu16* GBA::PaletteBank::s_BackgroundPalettes = ((vu16*)COLOUR_PALETTE_RAM);			// 512 colours
vu16* GBA::PaletteBank::s_SpritePalettes = ((vu16*)(COLOUR_PALETTE_RAM + 0x200));	// 512 colours

GBA::PaletteBlock16 * GBA::PaletteBank::GetPaletteBlock(vu16 * paletteLocation, u8 index)
{
	return index < c_BlockCount4bbp ? paletteLocation + index : nullptr;
}

void GBA::PaletteBank::LoadSpritePalette(u8 blockIndex, const ColourPalette16 & palette)
{
	PaletteBlock16* block = GetSpritePaletteBlock(blockIndex);
	LoadPalette(block, palette);
}

void GBA::PaletteBank::LoadSpritePalette(const ColourPalette256 & palette)
{
	PaletteBlock256* block = GetSpritePalette();
	LoadPalette(block, palette);
}
