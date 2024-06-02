#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"

namespace GBA
{
	namespace Gfx
	{
		using tTileId = u16;
		constexpr tTileId INVALID_TILE_ID = 0;

		namespace Tile
		{
			constexpr int PIXELS_SQRROOT_PER_TILE = 8;	// Tile are square size, same width and height
			constexpr int PIXELS_PER_TILE = PIXELS_SQRROOT_PER_TILE * PIXELS_SQRROOT_PER_TILE;
		}
	}
}
