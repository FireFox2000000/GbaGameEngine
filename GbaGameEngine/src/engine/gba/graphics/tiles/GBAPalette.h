#pragma once
#include "../../../base/Typedefs.h"
#include "../../../base/colour/Colour.h"

typedef vu16 PaletteBlock16;		// 4bbp
typedef vu16 PaletteBlock256;		// 8bbp

namespace GBA
{
	class Palette
	{
		static vu16* s_BackgroundPalettes;
		static vu16* s_SpritePalettes;

		static PaletteBlock16 * GetPaletteBlock(vu16* paletteLocation, u8 index);
		static void SetPaletteColour(PaletteBlock256* palette, u8 colourIndex, rbg16 colour);
		static void SetPalette16Colour(PaletteBlock16* paletteBlock, u8 colourIndex, rbg16 colour);

		static PaletteBlock16 * GetSpritePaletteBlock(u8 index) { return GetPaletteBlock(s_SpritePalettes, index); }
		static PaletteBlock256 * GetBackgroundPalette() { return GetPaletteBlock(s_BackgroundPalettes, 0); }
		static PaletteBlock256 * GetSpritePalette() { return GetPaletteBlock(s_SpritePalettes, 0); }

	public:
		static void SetSpritePaletteColour(u8 blockIndex, u8 colourIndex, rbg16 colour) { SetPalette16Colour(GetSpritePaletteBlock(blockIndex), colourIndex, colour); }
	};
}

