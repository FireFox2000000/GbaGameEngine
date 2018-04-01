/*
#include "../../../base/Typedefs.h"
#include <array>
#include <vector>

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

	typedef vu32 t8Pixels4bpp;
	typedef std::vector<t8Pixels4bpp> tTile4bpp;	// Tiles are 8x8 pixels. 32 bits = 4 bytes, 4 bytes * 2 pixels each * 8 rows = 1 tile
	typedef std::vector<tTile4bpp> tTileBlock;
	typedef std::vector<tTileBlock> tVRam;

	class TileBank
	{
		static tVRam * s_VRam;

		static tTileBlock* EditTileBlock(TileBlockGroups group) { return &((*s_VRam)[int(group)]); }

	public:
		static void LoadTiles(const std::vector<u16>& pixelMap, TileBlockGroups tileBlock, u32 startTileIndex);
	};
}
*/
