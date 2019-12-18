#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/graphicalassets/tile/Tile.h"

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

	typedef List<u32> tSpriteData;
	
	class TileBank
	{
		static const int CharBlockSize = 512;
		static const int CharBlock8Size = 256;

		
		typedef Array<Tile::Tile, CharBlockSize> CharBlock;
		typedef Array<Tile::Tile8, CharBlock8Size> CharBlock8;
		typedef Array<CharBlock, BlockGroupCount> CharBlockPool;
		typedef Array<CharBlock8, BlockGroupCount> CharBlockPool8;

		static volatile CharBlockPool & s_charBlockPool;
		static volatile CharBlockPool8 & s_charBlockPool8;
	
		static volatile CharBlock* EditTileBlock(TileBlockGroups group) { return &(s_charBlockPool[int(group)]); }
		static volatile CharBlock8* EditTileBlock8(TileBlockGroups group) { return &(s_charBlockPool8[int(group)]); }

	public:
		static bool LoadTiles(const u32* pixelMap, u32 pixelMapSize, TileBlockGroups tileBlockGroup, u16 startTileIndex);
		static bool LoadTiles(const List<u32>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex);

		template <u32 SIZE>
		static bool LoadTiles(const Array<u32, SIZE>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex)
		{
			return LoadTiles(pixelMap.GetContainer(), pixelMap.Count(), tileBlockGroup, startTileIndex);
		}

		// Load tiles that use 4bbp
		static bool LoadSpriteTiles(const tSpriteData& pixelMap, tTileId tileId);
	};
}
