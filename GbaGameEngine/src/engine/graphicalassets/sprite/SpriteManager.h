#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/colour/Palette.h"
#include "engine/graphicalassets/tile/Tile.h"

class Sprite;

class SpriteManager
{
	static const u32 MAX_SPRITE_TILES = 1024;

	enum TileReferenceState
	{
		Free,
		Used,
		Continue
	};

	Array<u32, sizeof(ColourPalette16) / sizeof(rgb16)> m_paletteRefTracker;
	Array<TileReferenceState, MAX_SPRITE_TILES> m_tileRefTracker;

	tTileId FindNextFreeTileSpace(u8 tileCount) const;

public:
	SpriteManager();
	~SpriteManager();

	void Load(Sprite& out_sprite);
	void Unload(Sprite* sprite);
	void Clear();
};
