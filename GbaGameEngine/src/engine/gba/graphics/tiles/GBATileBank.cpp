/*#include "GBATileBank.h"
#define TILE_MEM (0x06000000)

namespace GBA
{
	tVRam* TileBank::s_VRam = reinterpret_cast<tVRam*>(TILE_MEM);

	void TileBank::LoadTiles(const std::vector<u16>& pixelMap, TileBlockGroups tileBlockGroup, u32 startTileIndex)
	{
		tTileBlock& tileBlock = *(EditTileBlock(tileBlockGroup));
		std::size_t tileCount = pixelMap.size() / 16;	// 4 pixels for each u16, total of 64 pixels needed for a tile

		for (std::size_t tileIndex = 0; tileIndex < tileCount; ++tileIndex)
		{
			tTile4bpp& tile = tileBlock[startTileIndex + tileIndex];
			for (int pixelIndex = 0; pixelIndex < 16; pixelIndex += 2)
			{
				int pixelMapIndex = tileIndex * pixelIndex;
				tile[pixelIndex] = (pixelMap[pixelMapIndex] << 16) + pixelMap[pixelMapIndex + 1];
			}
		}
	}
}*/
