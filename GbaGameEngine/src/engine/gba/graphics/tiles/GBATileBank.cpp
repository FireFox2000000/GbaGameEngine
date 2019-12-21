#include "GBATileBank.h"
#include "engine/gba/memory/GBAMemoryLocations.h"

namespace GBA
{
	volatile TileBank::CharBlockPool& TileBank::s_charBlockPool = *reinterpret_cast<volatile CharBlockPool*>(VRAM);
	volatile TileBank::CharBlockPool8& TileBank::s_charBlockPool8 = *reinterpret_cast<volatile CharBlockPool8*>(VRAM);

	bool TileBank::LoadTiles(const u32 * pixelMap, u32 pixelMapSize, TileBlockGroups tileBlockGroup, u16 startTileIndex)
	{
		volatile Tile::Tile* tileBlock = EditTileBlock(tileBlockGroup)->At(0);
		vu32 *tileMem = reinterpret_cast<vu32*>(tileBlock + startTileIndex);			// Allow tiles to bleed over into further char blocks

		if ((void*)(tileMem + pixelMapSize) <= (void*)s_charBlockPool.end())
		{
			for (u32 i = 0; i < pixelMapSize; ++i)
			{
				tileMem[i] = *(pixelMap + i);
			}
			return true;
		}

		return false; // NOT ENOUGH SPACE, OUT OF VRAM!
	}

	bool TileBank::LoadTiles(const List<u32>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex)
	{
		return LoadTiles(pixelMap.begin(), pixelMap.Count(), tileBlockGroup, startTileIndex);
	}

	bool TileBank::LoadSpriteTiles(const tSpriteData& pixelMap, tTileId tileId)
	{
		if (tileId < CharBlockSize)
			return LoadTiles(pixelMap, SpriteLower, tileId);
		else
			return LoadTiles(pixelMap, SpriteHigher, tileId - CharBlockSize);
	}
}
