#include "GBATileBank.h"
#define TILE_MEM (0x06000000)

namespace GBA
{
	TileBank::TileVRam* TileBank::s_VRam = reinterpret_cast<TileVRam*>(TILE_MEM);
	TileBank::TileVRam8* TileBank::s_VRam8 = reinterpret_cast<TileVRam8*>(TILE_MEM);

	bool TileBank::LoadTiles(const List<u16>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex)
	{
		CharBlock& tileBlock = *(EditTileBlock(tileBlockGroup));	
		volatile u16 *tileMem = (u16*)&tileBlock[startTileIndex];	

		if ((void*)(tileMem + pixelMap.Count()) <= (void*)s_VRam->end())
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
