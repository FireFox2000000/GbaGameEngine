#include "AssetLoadFunctions.h"
#include "engine/math/Math.h"
#include "engine/io/filestream/MemoryMappedFileStream.h"
#include "engine/graphics/Graphics.h"
#include "engine/engine/Engine.h"
#include "engine/base/core/stl/Pool.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"
#include "gbatek/Vram.h"

using namespace GBA::Gfx;

TilemapSet* AssetLoadFunctions::CreateTilemapSetFromFile(MemoryMappedFileStream& istream, IPool<TilemapSet>& allocator)
{
	// Read palette
	u8 paletteBankIndexOffset = istream.Read<u8>();
	u8 paletteLength = istream.Read<u8>();
	Span<const GBATEK::ColourRGB16> palette = istream.Read<GBATEK::ColourRGB16>(paletteLength);

	// Read tileset
	u32 compressionFlags = istream.Read<u32>();

	u32 tilesetLength = istream.Read<u32>();
	Span<const GBATEK::UPixelData> tileset = istream.Read<GBATEK::UPixelData>(tilesetLength);

	// Read maps
	u8 mapCount = istream.Read<u8>();
	int tileMapDataLength = istream.Read<int>();
	u8 mapIsDynamicMask = istream.Read<u8>();
	Span<const u8> widthMap = istream.Read<u8>(mapCount);
	Span<const u8> heightMap = istream.Read<u8>(mapCount);
	Span<const GBATEK::BackgroundTilemapEntry> tilemapEntries = istream.Read<GBATEK::BackgroundTilemapEntry>(tileMapDataLength);

	DEBUG_LOGFORMAT("Loaded tilemap set of size %.2fkb", BYTES_TO_KB(tilesetLength * sizeof(u32)));
	DEBUG_LOGFORMAT("Loaded tilemap data of size %.2fkb", BYTES_TO_KB(tileMapDataLength * sizeof(u16)));

	return allocator.Create(TilemapSet(
		paletteBankIndexOffset
		, palette
		, tileset
		, compressionFlags
		, mapCount
		, mapIsDynamicMask
		, widthMap
		, heightMap
		, tilemapEntries
	));
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
