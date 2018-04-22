#ifndef PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_TILES_GBATILE_H
#define PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_TILES_GBATILE_H

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/math/Vector2.h"

namespace GBA
{
	namespace TileConfig
	{
		const int PIXELS_SQRROOT_PER_TILE = 8;
		const int PIXELS_PER_TILE = PIXELS_SQRROOT_PER_TILE * PIXELS_SQRROOT_PER_TILE;
	};
}

#endif
