#include "GBAPalette.h"
#include "../../memory/GBAMemoryLocations.h"
#include "../../../base/Macros.h"

const u8 c_BlockCount4bbp = 16;
const u8 c_ColourCount4bbp = 16;

vu16* GBA::Palette::s_BackgroundPalettes = ((vu16*)COLOUR_PALETTE_RAM);			// 512 colours
vu16* GBA::Palette::s_SpritePalettes = ((vu16*)(COLOUR_PALETTE_RAM + 0x200));	// 512 colours

PaletteBlock16 * GBA::Palette::GetPaletteBlock(vu16 * paletteLocation, u8 index)
{
	return index < c_BlockCount4bbp ? paletteLocation + index : nullptr;
}

void GBA::Palette::SetPaletteColour(PaletteBlock256 * palette, u8 colourIndex, rbg16 colour)
{
	palette[colourIndex] = colour;		// No need to check if colour index is less than 256 because we're passing in a u8
}

void GBA::Palette::SetPalette16Colour(PaletteBlock16 * paletteBlock, u8 colourIndex, rbg16 colour)
{
	if (paletteBlock && colourIndex < c_ColourCount4bbp)
		SetPaletteColour(paletteBlock, colourIndex, colour);
}