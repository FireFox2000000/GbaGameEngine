
#include "../../../base/Typedefs.h"
#include "../../../base/core/stl/Array.h"
//#include <vector>

namespace GBA
{
	enum TileBlockGroups
	{
		Bg0,
		Bg1,
		Bg2,
		Bg3,
		SpriteLower,		// May be used in bitmap modes
		SpriteHigher,

		BlockGroupCount
	};

	typedef u32 tPixelIndex4bppX8;
	typedef u32 tPixelIndex8bppX4;
	typedef Array<tPixelIndex4bppX8, 8> Tile4bpp;	// Tiles are 8x8 pixels. 32 bits = 4 bytes, 4 bytes * 2 pixels per byte * 8 rows = 1 tile
	typedef Array<tPixelIndex8bppX4, 16> Tile8bpp;
	typedef Array<Tile4bpp, 512> SpriteTileBlock4bbp;
	typedef Array<Tile4bpp, 256> SpriteTileBlock8bbp;
	//typedef std::vector<tTileBlock> tVRam;
	//
	//class TileBank
	//{
	//	static tVRam * s_VRam;
	//
	//	static tTileBlock* EditTileBlock(TileBlockGroups group) { return &((*s_VRam)[int(group)]); }
	//
	//public:
	//	static void LoadTiles(const std::vector<u16>& pixelMap, TileBlockGroups tileBlock, u32 startTileIndex);
	//};
}
