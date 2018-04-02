#include "GBAPaletteBank.h"
#include "../../memory/GBAMemoryLocations.h"
#include "../../../base/Macros.h"

const u8 c_BlockCount4bbp = 16;
const u8 c_ColourCount4bbp = 16;

namespace GBA
{
	PaletteBank::vColourPalette256 *	PaletteBank::s_FullBackgroundPalette		= (reinterpret_cast<PaletteBank::vColourPalette256*>(COLOUR_PALETTE_RAM));
	PaletteBank::vColourPalette16x16 *	PaletteBank::s_BackgroundPalette16Groups	= (reinterpret_cast<PaletteBank::vColourPalette16x16*>(COLOUR_PALETTE_RAM));			// 512 colours
	PaletteBank::vColourPalette256 *	PaletteBank::s_FullSpritePalette			= (reinterpret_cast<PaletteBank::vColourPalette256*>((COLOUR_PALETTE_RAM + 0x200)));	// 512 colours
	PaletteBank::vColourPalette16x16 *	PaletteBank::s_SpritePalette16Groups		= (reinterpret_cast<PaletteBank::vColourPalette16x16*>((COLOUR_PALETTE_RAM + 0x200)));	// 512 colours
}
