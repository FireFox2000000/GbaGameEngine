#include "TilemapLoadFunctions.h"
#include "engine/math/Math.h"
#include "engine/filestream/CppFileReader.h"

TilemapSet TilemapLoadFunctions::CreateTilemapSetFromFile(const u32* file)
{
	CppFileReader reader = CppFileReader(file);

	// Read palette
	u8 paletteLength = reader.Read<u8>();
	u16 * palette = reader.ReadAddress<u16>(paletteLength);

	// Read tileset
	u32 compressionFlags = reader.Read<u32>();

	u32 tilesetLength = reader.Read<u32>();
	u32* tileset = reader.ReadAddress<u32>(tilesetLength);

	// Read maps
	u8 mapCount = reader.Read<u8>();
	int tileMapDataLength = reader.Read<int>();
	u8 mapIsDynamicMask = reader.Read<u8>();
	u8* widthMap = reader.ReadAddress<u8>(mapCount);
	u8* heightMap = reader.ReadAddress<u8>(mapCount);
	u16* mapData = reader.ReadAddress<u16>(tileMapDataLength);

	return TilemapSet(paletteLength, palette, tilesetLength, tileset, compressionFlags, mapCount, mapIsDynamicMask, widthMap, heightMap, mapData);
}
