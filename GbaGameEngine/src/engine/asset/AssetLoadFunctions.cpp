#include "AssetLoadFunctions.h"
#include "engine/math/Math.h"
#include "engine/io/filestream/CppFileReader.h"
#include "engine/graphics/Graphics.h"
#include "engine/engine/Engine.h"
#include "engine/base/core/stl/Pool.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"
#include "gbatek/Vram.h"

using namespace GBA::Gfx;

TilemapSet AssetLoadFunctions::CreateTilemapSetFromFile(const u32* file)
{
	CppFileReader reader = CppFileReader(file);

	// Read palette
	u8 paletteBankIndexOffset = reader.Read<u8>();
	u8 paletteLength = reader.Read<u8>();
	Span<const GBATEK::ColourRGB16> palette = reader.ReadSpan<GBATEK::ColourRGB16>(paletteLength);

	// Read tileset
	u32 compressionFlags = reader.Read<u32>();

	u32 tilesetLength = reader.Read<u32>();
	Span<const GBATEK::UPixelData> tileset = reader.ReadSpan<GBATEK::UPixelData>(tilesetLength);

	// Read maps
	u8 mapCount = reader.Read<u8>();
	int tileMapDataLength = reader.Read<int>();
	u8 mapIsDynamicMask = reader.Read<u8>();
	Span<const u8> widthMap = reader.ReadSpan<u8>(mapCount);
	Span<const u8> heightMap = reader.ReadSpan<u8>(mapCount);
	Span<const GBATEK::BackgroundTilemapEntry> tilemapEntries = reader.ReadSpan<GBATEK::BackgroundTilemapEntry>(tileMapDataLength);

	DEBUG_LOGFORMAT("Loaded tilemap set of size %.2fkb", BYTES_TO_KB(tilesetLength * sizeof(u32)));
	DEBUG_LOGFORMAT("Loaded tilemap data of size %.2fkb", BYTES_TO_KB(tileMapDataLength * sizeof(u16)));

	return TilemapSet(
		paletteBankIndexOffset
		, palette
		, tileset
		, compressionFlags
		, mapCount
		, mapIsDynamicMask
		, widthMap
		, heightMap
		, tilemapEntries
	);
}

void AssetLoadFunctions::Unload(Engine* engine, TilemapSet * begin, TilemapSet * end)
{
	Graphics* graphicsManager = engine->GetComponent<Graphics>();

	for (auto* tilemapSet = begin; tilemapSet != end; ++tilemapSet)
	{
		for (auto& tilemap : tilemapSet->m_maps)
		{
			graphicsManager->Unload(&tilemap);
		}
	}
}
