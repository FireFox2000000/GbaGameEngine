#include "GBASpriteAtlasGfxData.h"

namespace GBA
{
	namespace Gfx
	{
		bool SpriteAtlasGfxData::IsPaletteLoaded() const
		{
			return GetPaletteIndex() != INVALID_PALETTE_INDEX;
		}
	}
}
