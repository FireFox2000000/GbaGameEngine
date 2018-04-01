#pragma once
#include "../../../base/Typedefs.h"
#include "../../../base/colour/Colour.h"
#include <array>

namespace GBA
{
	typedef vu16 PaletteBlock16;		// 4bbp
	typedef vu16 PaletteBlock256;		// 8bbp

	typedef std::array<rgb16, 16> ColourPalette16;
	typedef std::array<rgb16, 256> ColourPalette256;

	class PaletteBank
	{
		static vu16* s_BackgroundPalettes;
		static vu16* s_SpritePalettes;

		static PaletteBlock16 * GetPaletteBlock(vu16* paletteLocation, u8 index);
		inline static void SetPaletteColour(vu16* palette, u8 colourIndex, rgb16 colour) { palette[colourIndex] = colour; }		// No need to check if colour index is less than 256 because we're passing in a u8		

		static PaletteBlock16 * GetSpritePaletteBlock(u8 index) { return GetPaletteBlock(s_SpritePalettes, index); }
		static PaletteBlock256 * GetBackgroundPalette() { return GetPaletteBlock(s_BackgroundPalettes, 0); }
		static PaletteBlock256 * GetSpritePalette() { return GetPaletteBlock(s_SpritePalettes, 0); }

		template<std::size_t SIZE>
		static void LoadPalette(vu16* block, const std::array<rgb16, SIZE>& palette)
		{
			if (block)
			{
				for (std::size_t i = 0; i < palette.size(); ++i)
				{
					SetPaletteColour(block, i, palette[i]);
				}
			}
		}

	public:
		static void LoadSpritePalette(u8 blockIndex, const ColourPalette16& palette);
		static void LoadSpritePalette(const ColourPalette256& palette);
	};
}

