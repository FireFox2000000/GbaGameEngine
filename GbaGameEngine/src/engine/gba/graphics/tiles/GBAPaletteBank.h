#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/colour/Palette.h"
#include "GBASDK/ColourPalettes.h"

namespace GBA
{
	namespace Gfx
	{
		// Wrapper around the physical GBA palette bank.
		class PaletteBank
		{
		public:
			typedef Array<ColourPalette16, 16> ColourPalette16x16;

		public:
			static void LoadBackgroundPalette(tPaletteIndex blockIndex, const ColourPalette16& palette);
			static void LoadBackgroundPalette(const ColourPalette256& palette);
			static void LoadBackgroundPalette(const u16* palette, u8 arrLength);
			static void LoadBackgroundPalette(tPaletteIndex blockIndex, const u16* palette, u8 arrLength);

			static void LoadSpritePalette(tPaletteIndex blockIndex, const ColourPalette16& palette);
			static void LoadSpritePalette(const ColourPalette256& palette);
		
			static GBA::ColourPalette* EditSpritePalette();
			static GBA::ColourPalette* EditBackgroundPalette();
		};
	}
}
