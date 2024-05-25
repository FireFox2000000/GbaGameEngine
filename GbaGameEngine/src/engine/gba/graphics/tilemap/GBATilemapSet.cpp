#include "GBATilemapSet.h"
#include "engine/base/core/stl/Bitmask.h"
#include "GBASDK/Vram.h"

using namespace GBA::Gfx;

TilemapSet::TilemapSet()
{

}

TilemapSet::TilemapSet(
	const u8 paletteBankIndex
	, const u8 paletteLength
	, const ColourRGB16* palette
	, const u32 tilesetLength
	, const GBA::UPixelData* tileset
	, const u32 tileSetDataCompressionFlags
	, const u8 mapCount
	, const u8 mapIsDynamicMask
	, const u8* mapTileWidths
	, const u8* mapTileHeights
	, const GBA::BackgroundTilemapEntry* mapData
)
{
	m_file.m_paletteBankIndex = paletteBankIndex;
	m_file.m_paletteLength = paletteLength;
	m_file.m_palette = palette;

	m_file.m_tileSetDataCompressionFlags = tileSetDataCompressionFlags;
	m_file.m_tilesetLength = tilesetLength;
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

		tilemap->m_file.m_tileMapData = &mapData[mapDataOffset];
		tilemap->m_file.m_tileMapDataLength = totalTiles;
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
