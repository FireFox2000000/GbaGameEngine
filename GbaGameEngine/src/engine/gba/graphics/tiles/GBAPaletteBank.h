#pragma once

#include "../../../base/Typedefs.h"
#include "../../../base/colour/Colour.h"
#include "../../../base/stl/FixedArray.h"

namespace GBA
{
	typedef FixedArray<rgb16, 16> ColourPalette16;
	typedef FixedArray<rgb16, 256> ColourPalette256;

	class PaletteBank
	{
		typedef FixedArray<volatile rgb16, 16> vColourPalette16;
		typedef FixedArray<volatile rgb16, 256> vColourPalette256;
		typedef FixedArray<vColourPalette16, 16> vColourPalette16x16;

		static vColourPalette16x16* s_BackgroundPalette16Groups;
		static vColourPalette256* s_FullBackgroundPalette;
		static vColourPalette16x16* s_SpritePalette16Groups;
		static vColourPalette256* s_FullSpritePalette;

		template<u32 SIZE>
		static void LoadPalette(FixedArray<volatile rgb16, SIZE>* blockPtr, const FixedArray<rgb16, SIZE>& palette)
		{
			if (blockPtr)
			{
				FixedArray<volatile rgb16, SIZE>& block = *blockPtr;
				for (u32 i = 0; i < palette.Length(); ++i)
				{
					block[i] = palette[i];
				}
			}
		}

	public:
		static void LoadBackgroundPalette(u8 blockIndex, const ColourPalette16& palette) { LoadPalette(s_BackgroundPalette16Groups->At(blockIndex), palette); }
		static void LoadBackgroundPalette(const ColourPalette256& palette) { LoadPalette(s_FullBackgroundPalette, palette); }

		static void LoadSpritePalette(u8 blockIndex, const ColourPalette16& palette) { LoadPalette(s_SpritePalette16Groups->At(blockIndex), palette); }
		static void LoadSpritePalette(const ColourPalette256& palette) { LoadPalette(s_FullSpritePalette, palette); }
	};
}
