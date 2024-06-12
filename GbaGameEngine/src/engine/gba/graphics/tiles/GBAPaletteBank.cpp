#include "GBAPaletteBank.h"
#include "engine/base/Macros.h"
#include "engine/base/core/Memory.h"

constexpr int c_BlockCount4bbp = 16;
constexpr int c_ColourCount4bbp = 16;

template<u32 SIZE>
static void LoadPalette(GBATEK::ColourRGB16* block, const Array<GBATEK::ColourRGB16, SIZE>& palette)
{
	if (block)
	{
		VramSafeMemCopy(block, palette.GetContainer(), SIZE);
	}
}

namespace GBA
{
	namespace Gfx
	{
		void PaletteBank::LoadBackgroundPalette(tPaletteIndex blockIndex, const ColourPalette16& palette)
		{
			LoadPalette((*GBATEK::colourPaletteBackgrounds) + (blockIndex * 16), palette);
		}

		void PaletteBank::LoadBackgroundPalette(const ColourPalette256& palette)
		{
			LoadPalette(*GBATEK::colourPaletteBackgrounds, palette);
		}

		// 256 colours
		void PaletteBank::LoadBackgroundPalette(const GBATEK::ColourRGB16* palette, u8 arrLength)
		{
			VramSafeMemCopy(*GBATEK::colourPaletteBackgrounds, palette, arrLength);
		}

		void PaletteBank::LoadBackgroundPalette(tPaletteIndex blockIndex, const GBATEK::ColourRGB16* palette, u8 arrLength)
		{
			VramSafeMemCopy((*GBATEK::colourPaletteBackgrounds) + (blockIndex * c_ColourCount4bbp), palette, arrLength);
		}

		void PaletteBank::LoadSpritePalette(tPaletteIndex blockIndex, const ColourPalette16& palette)
		{
			LoadPalette((*GBATEK::colourPaletteObjects) + (blockIndex * c_ColourCount4bbp), palette);
		}

		void PaletteBank::LoadSpritePalette(const ColourPalette256& palette)
		{
			LoadPalette(*GBATEK::colourPaletteObjects, palette);
		}

		GBATEK::ColourPalette* PaletteBank::EditSpritePalette()
		{
			return GBATEK::colourPaletteObjects;
		}

		GBATEK::ColourPalette* PaletteBank::EditBackgroundPalette()
		{
			return GBATEK::colourPaletteBackgrounds;
		}
	}
}
