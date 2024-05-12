#pragma once
namespace GBA
{
	struct ColourRGB16
	{
		// 0 - 31
		unsigned short r : 5;

		// 0 - 31
		unsigned short g : 5;

		// 0 - 31
		unsigned short b : 5;

		// This exists for fast colour comparisons and VRAM mem copies
		inline operator unsigned short() const { return *reinterpret_cast<const u16*>(this); }
	};

	static_assert(sizeof(ColourRGB16) == sizeof(unsigned short), "ColourRGB16 struct malformed");

	// The first colour in each block of all BG and OBJ palettes is transparent. 
	// Even though palettes are described as 16 (256) color palettes, only 15 (255) colors are actually visible.

	using ColourPalette = ColourRGB16[256];

	ColourPalette* const colourPaletteBackgrounds = reinterpret_cast<ColourPalette*>(0x05000000);
	ColourPalette* const colourPaletteObjects = reinterpret_cast<ColourPalette*>(0x05000200);
}
