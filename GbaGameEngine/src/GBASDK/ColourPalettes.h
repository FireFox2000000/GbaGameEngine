#pragma once
#include "ColourRGB16.h"

namespace GBA
{
	// The first colour in each block of all BG and OBJ palettes is transparent. 
	// Even though palettes are described as 16 (256) color palettes, only 15 (255) colors are actually visible.

	using ColourPalette = ColourRGB16[256];

	ColourPalette* const colourPaletteBackgrounds = reinterpret_cast<ColourPalette*>(0x05000000);
	ColourPalette* const colourPaletteObjects = reinterpret_cast<ColourPalette*>(0x05000200);
}
