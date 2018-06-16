#ifndef PRAGMA_ONCE_ENGINE_GRAPHICALASSETS_TILE_TILE_H
#define PRAGMA_ONCE_ENGINE_GRAPHICALASSETS_TILE_TILE_H

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/math/Vector2.h"

typedef u16 tTileId;
const tTileId INVALID_TILE_ID = 0;

namespace Tile 
{
	const int PIXELS_SQRROOT_PER_TILE = 8;	// Tile are square size, same width and height
	const int PIXELS_PER_TILE = PIXELS_SQRROOT_PER_TILE * PIXELS_SQRROOT_PER_TILE;

	typedef u32 tPixelIndex4bppX8;
	typedef u32 tPixelIndex8bppX4;
	typedef Array<tPixelIndex4bppX8, PIXELS_PER_TILE / 8> Tile;	// Tiles are 8x8 pixels. 32 bits = 4 bytes, 4 bytes * 2 pixels per byte * 8 rows = 1 tile
	typedef Array<tPixelIndex8bppX4, PIXELS_PER_TILE / 4> Tile8;
}

#endif
