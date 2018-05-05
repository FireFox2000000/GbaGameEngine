#ifndef PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_TILES_GBATILEBANK_H
#define PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_TILES_GBATILEBANK_H

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "GBATileConfig.h"

namespace GBA
{
	enum TileBlockGroups
	{
		Bg0,
		Bg1,
		Bg2,
		Bg3,
		SpriteLower,		// May be used by backgrounds in bitmap modes
		SpriteHigher,

		BlockGroupCount
	};

	typedef u16 tTileId;
	typedef List<u16> tSpriteData;
	
	class TileBank
	{
		static const int CharBlockSize = 512;
		static const int CharBlock8Size = 256;

		typedef u32 tPixelIndex4bppX8;
		typedef u32 tPixelIndex8bppX4;
		typedef Array<tPixelIndex4bppX8, TileConfig::PIXELS_PER_TILE / 8> Tile;	// Tiles are 8x8 pixels. 32 bits = 4 bytes, 4 bytes * 2 pixels per byte * 8 rows = 1 tile
		typedef Array<tPixelIndex8bppX4, TileConfig::PIXELS_PER_TILE / 4> Tile8;
		typedef Array<Tile, CharBlockSize> CharBlock;
		typedef Array<Tile8, CharBlock8Size> CharBlock8;
		typedef Array<CharBlock, BlockGroupCount> CharBlockPool;
		typedef Array<CharBlock8, BlockGroupCount> CharBlockPool8;

		static volatile CharBlockPool & s_charBlockPool;
		static volatile CharBlockPool8 & s_charBlockPool8;
	
		static volatile CharBlock* EditTileBlock(TileBlockGroups group) { return &(s_charBlockPool[int(group)]); }
		static volatile CharBlock8* EditTileBlock8(TileBlockGroups group) { return &(s_charBlockPool8[int(group)]); }

	public:
		static bool LoadTiles(const u16* pixelMap, u32 pixelMapSize, TileBlockGroups tileBlockGroup, u16 startTileIndex);
		static bool LoadTiles(const List<u16>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex);

		template <u32 SIZE>
		static bool LoadTiles(const Array<u16, SIZE>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex)
		{
			return LoadTiles(pixelMap.GetContainer(), pixelMap.Count(), tileBlockGroup, startTileIndex);
		}

		// Load tiles that use 4bbp
		static bool LoadSpriteTiles(const tSpriteData& pixelMap, tTileId tileId);
	};
}

#endif
