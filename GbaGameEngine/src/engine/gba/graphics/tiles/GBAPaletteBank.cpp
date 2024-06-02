#include "GBAPaletteBank.h"
#include "engine/base/Macros.h"
#include "engine/base/core/Memory.h"

constexpr int c_BlockCount4bbp = 16;
constexpr int c_ColourCount4bbp = 16;

template<u32 SIZE>
static void LoadPalette(GBA::ColourRGB16* block, const Array<GBA::ColourRGB16, SIZE>& palette)
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
			LoadPalette((*GBA::colourPaletteBackgrounds) + (blockIndex * 16), palette);
		}

		void PaletteBank::LoadBackgroundPalette(const ColourPalette256& palette)
		{
			LoadPalette(*GBA::colourPaletteBackgrounds, palette);
		}

		// 256 colours
		void PaletteBank::LoadBackgroundPalette(const ColourRGB16* palette, u8 arrLength)
		{
			VramSafeMemCopy(*GBA::colourPaletteBackgrounds, palette, arrLength);
		}

		void PaletteBank::LoadBackgroundPalette(tPaletteIndex blockIndex, const ColourRGB16* palette, u8 arrLength)
		{
			VramSafeMemCopy((*GBA::colourPaletteBackgrounds) + (blockIndex * c_ColourCount4bbp), palette, arrLength);
		}

		void PaletteBank::LoadSpritePalette(tPaletteIndex blockIndex, const ColourPalette16& palette)
		{
			LoadPalette((*GBA::colourPaletteObjects) + (blockIndex * c_ColourCount4bbp), palette);
		}

		void PaletteBank::LoadSpritePalette(const ColourPalette256& palette)
		{
			LoadPalette(*GBA::colourPaletteObjects, palette);
		}

		GBA::ColourPalette* PaletteBank::EditSpritePalette()
		{
			return GBA::colourPaletteObjects;
		}

		GBA::ColourPalette* PaletteBank::EditBackgroundPalette()
		{
			return GBA::colourPaletteBackgrounds;
		}
	}
}
