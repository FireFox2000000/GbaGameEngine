#pragma once

#include "../../../base/Typedefs.h"
#include "../../../base/colour/Colour.h"
#include "../../../base/core/stl/Array.h"

namespace GBA
{
	typedef Array<rgb16, 16> ColourPalette16;
	typedef Array<rgb16, 256> ColourPalette256;

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
		static void LoadBackgroundPalette(u8 blockIndex, const ColourPalette16& palette) { LoadPalette(s_BackgroundPalette16Groups->At(blockIndex), palette); }
		static void LoadBackgroundPalette(const ColourPalette256& palette) { LoadPalette(s_FullBackgroundPalette, palette); }
		
		static void LoadSpritePalette(u8 blockIndex, const ColourPalette16& palette) { LoadPalette(s_SpritePalette16Groups->At(blockIndex), palette); }
		static void LoadSpritePalette(const ColourPalette256& palette) { LoadPalette(s_FullSpritePalette, palette); }
	};
}
