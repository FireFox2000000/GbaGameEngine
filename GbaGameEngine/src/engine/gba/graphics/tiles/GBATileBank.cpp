#include "GBATileBank.h"
#define TILE_MEM (0x06000000)

namespace GBA
{
	volatile TileBank::TileVRam& TileBank::s_VRam = *reinterpret_cast<volatile TileVRam*>(TILE_MEM);
	volatile TileBank::TileVRam8& TileBank::s_VRam8 = *reinterpret_cast<volatile TileVRam8*>(TILE_MEM);

	bool TileBank::LoadTiles(const List<u16>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex)
	{
		volatile CharBlock* tileBlock = EditTileBlock(tileBlockGroup);	
		vu16 *tileMem = reinterpret_cast<vu16*>(tileBlock->At(startTileIndex));

		if ((void*)(tileMem + pixelMap.Count()) <= (void*)s_VRam.end())
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
