#include "GBATileBank.h"
#define TILE_MEM (0x06000000)

namespace GBA
{
	volatile TileBank::CharBlockPool& TileBank::s_charBlockPool = *reinterpret_cast<volatile CharBlockPool*>(TILE_MEM);
	volatile TileBank::CharBlockPool8& TileBank::s_charBlockPool8 = *reinterpret_cast<volatile CharBlockPool8*>(TILE_MEM);

	bool TileBank::LoadTiles(const List<u16>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex)
	{
		volatile CharBlock* tileBlock = EditTileBlock(tileBlockGroup);	
		vu16 *tileMem = reinterpret_cast<vu16*>(tileBlock->At(startTileIndex));

		if ((void*)(tileMem + pixelMap.Count()) <= (void*)s_charBlockPool.end())
		{
			for (u32 i = 0; i < pixelMap.Count(); ++i)
			{
				tileMem[i] = pixelMap[i];
			}
			return true;
		}
		
		return false; // NOT ENOUGH SPACE, OUT OF VRAM!
	}

	bool TileBank::LoadSpriteTiles(const tSpriteData& pixelMap, tTileId tileId)
	{
		if (tileId < CharBlock::Count())
			return LoadTiles(pixelMap, SpriteLower, tileId);
		else
			return LoadTiles(pixelMap, SpriteHigher, tileId - CharBlock::Count());
	}
}
