#include "GBAPaletteBank.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/base/Macros.h"
#include "engine/base/core/Memory.h"

constexpr int c_BlockCount4bbp = 16;
constexpr int c_ColourCount4bbp = 16;

template<u32 SIZE>
static void LoadPalette(GBA::ColourRGB16* block, const Array<GBA::ColourRGB16, SIZE>& palette)
{
	if (block)
	{
		const GBA::ColourRGB16* src = palette.GetContainer();
		VramSafeMemCopy(block, src, SIZE * sizeof(*block));
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
		void PaletteBank::LoadBackgroundPalette(const u16* palette, u8 arrLength)
		{
			VramSafeMemCopy(GBA::colourPaletteBackgrounds, palette, arrLength * sizeof(u16));
		}

		void PaletteBank::LoadBackgroundPalette(tPaletteIndex blockIndex, const u16 * palette, u8 arrLength)
		{
			VramSafeMemCopy((*GBA::colourPaletteBackgrounds) + (blockIndex * c_ColourCount4bbp), palette, arrLength * sizeof(u16));
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
