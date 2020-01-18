#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/colour/Palette.h"

namespace GBA
{
	class PaletteBank
	{
		typedef Array<ColourPalette16, 16> ColourPalette16x16;

		static volatile ColourPalette16x16* s_BackgroundPalette16Groups;
		static volatile ColourPalette16x16* s_SpritePalette16Groups;
		static volatile ColourPalette256* s_FullBackgroundPalette;	
		static volatile ColourPalette256* s_FullSpritePalette;

		template<u32 SIZE>
		static void LoadPalette(volatile Array<rgb16, SIZE>* block, const Array<rgb16, SIZE>& palette)
		{
			if (block)
			{
				*block = palette;
			}
		}

	public:
		static void LoadBackgroundPalette(tPaletteIndex blockIndex, const ColourPalette16& palette) { LoadPalette(s_BackgroundPalette16Groups->At(blockIndex), palette); }
		static void LoadBackgroundPalette(const ColourPalette256& palette) { LoadPalette(s_FullBackgroundPalette, palette); }
		static void LoadBackgroundPalette(const u16* palette, u8 arrLength);
		static void LoadBackgroundPalette(tPaletteIndex blockIndex, const u16* palette, u8 arrLength);
		
		static void LoadSpritePalette(tPaletteIndex blockIndex, const ColourPalette16& palette) { LoadPalette(s_SpritePalette16Groups->At(blockIndex), palette); }
		static void LoadSpritePalette(const ColourPalette256& palette) { LoadPalette(s_FullSpritePalette, palette); }
	};
}
