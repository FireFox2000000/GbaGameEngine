#include "GBATilemapSet.h"
#include "engine/base/core/stl/Bitmask.h"
#include "engine/algorithm/Compression.h"
#include "gbatek/Vram.h"
#include "gbatek/Backgrounds.h"

using namespace GBA::Gfx;

TilemapSet::TilemapSet()
{

}

TilemapSet::TilemapSet(
	const u8 paletteBankIndex
	, const Span<const GBATEK::ColourRGB16> palette
	, const Span<const GBATEK::UPixelData> tileset
	, const u32 tileSetDataCompressionFlags
	, const u8 mapCount
	, const u8 mapIsDynamicMask
	, const Span<const u8> mapTileWidths
	, const Span<const u8> mapTileHeights
	, const Span<const GBATEK::BackgroundTilemapEntry> mapData
)
{
	DEBUG_ASSERTMSG(mapTileWidths.Count() == mapTileHeights.Count(), "Map tile widths and height mismatch");

	m_file.m_paletteBankIndex = paletteBankIndex;
	m_file.m_palette = palette;

	m_file.m_backgroundColourMode = Compression::GetBitPackedSrcBpp(tileSetDataCompressionFlags) > 4 ? GBATEK::BackgroundColourMode::Mode256x1 : GBATEK::BackgroundColourMode::Mode16x16;
	m_file.m_tileset = tileset;

	m_maps.Reserve(mapCount);

	Bitmask<u8> mapIsDynamicBitMask = Bitmask<u8>(mapIsDynamicMask);

	int mapDataOffset = 0;
	for (u32 i = 0; i < mapCount; ++i)
	{
		Tilemap* tilemap = m_maps.AddNew();
		tilemap->m_tilemapSet = this;
		tilemap->m_file.m_isDynamicallyRendered = mapIsDynamicBitMask.TestBit(i);

		u8 widthInTiles = mapTileWidths[i];
		u8 heightInTiles = mapTileHeights[i];
		u16 totalTiles = widthInTiles * heightInTiles;

		tilemap->m_file.m_sizeInTiles = Vector2<u8>(widthInTiles, heightInTiles);
		tilemap->m_file.m_tileMapEntries = { &mapData[mapDataOffset], totalTiles };

		mapDataOffset += totalTiles;
	}
}

TilemapSet::TilemapSet(const TilemapSet & that)
{
	*this = that;
	for (Tilemap& map : m_maps)
	{
		map.m_tilemapSet = this;
	}
}

bool TilemapSet::IsPaletteLoaded() const
{
	return m_renderData.m_paletteIndex != INVALID_PALETTE_INDEX;
}

tPaletteIndex GBA::Gfx::TilemapSet::GetPaletteIndex() const
{
	return m_renderData.m_paletteIndex;
}

bool TilemapSet::IsTileSetLoaded() const
{
	return GetTileSetCharacterBaseBlock() != INVALID_TILESET_CBB;
}

GBA::TileBlockGroups GBA::Gfx::TilemapSet::GetTileSetCharacterBaseBlock() const
{
	return m_renderData.m_tileSetCharacterBaseBlock;
}

bool TilemapSet::IsLoaded() const
{
	return IsPaletteLoaded() && IsTileSetLoaded();
}

Tilemap* GBA::Gfx::TilemapSet::GetTilemap(int index)
{
	if (index >= 0 && index < (int)m_maps.Count())
		return &m_maps[index];
	else
		return nullptr;
}

const Tilemap* GBA::Gfx::TilemapSet::GetTilemap(int index) const
{
	if (index >= 0 && index < (int)m_maps.Count())
		return &m_maps[index];
	else
		return nullptr;
}
