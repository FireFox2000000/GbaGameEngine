#include "AssetLoadFunctions.h"
#include "engine/math/Math.h"
#include "engine/filestream/CppFileReader.h"
#include "engine/graphicalassets/Graphics.h"
#include "engine/engine/Engine.h"

using namespace GBA::Gfx;

SpriteAtlus AssetLoadFunctions::CreateSpriteAtlusFromFile(const u32 * file)
{
	CppFileReader reader = CppFileReader(file);

	const u32 spriteCount = reader.Read<u32>();
	const u8 paletteLength = reader.Read<u8>();
	const u32 dataLength = reader.Read<u32>();
	const u32 compressionFlags = reader.Read<u32>();
	const u16 * palette = reader.ReadAddress<u16>(paletteLength);
	const u8 * widthMap = reader.ReadAddress<u8>(spriteCount);
	const u8 * heightMap = reader.ReadAddress<u8>(spriteCount);
	const u32 * offsets = reader.ReadAddress<u32>(spriteCount);
	const u32 * data = reader.ReadAddress<u32>(dataLength);

	DEBUG_LOGFORMAT("Loaded sprite atlus of size %.2fkb", BYTES_TO_KB(dataLength));

	return SpriteAtlus(spriteCount,
		paletteLength,
		palette,
		widthMap,
		heightMap,
		dataLength,
		compressionFlags,
		data,
		offsets);
}

TilemapSet AssetLoadFunctions::CreateTilemapSetFromFile(const u32* file)
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

	DEBUG_LOGFORMAT("Loaded tilemap set of size %.2fkb", BYTES_TO_KB(tilesetLength));

	return TilemapSet(paletteLength, palette, tilesetLength, tileset, compressionFlags, mapCount, mapIsDynamicMask, widthMap, heightMap, mapData);
}

void AssetLoadFunctions::Unload(Engine* engine, TilemapSet * begin, TilemapSet * end)
{
	Graphics* graphicsManager = engine->EditComponent<Graphics>();

	for (auto* tilemapSet = begin; tilemapSet != end; ++tilemapSet)
	{
		for (auto& tilemap : tilemapSet->m_maps)
		{
			graphicsManager->Unload(&tilemap);
		}
	}
}
