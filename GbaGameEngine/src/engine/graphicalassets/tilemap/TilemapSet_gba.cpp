#include "TilemapSet.h"

TilemapSet::TilemapSet()
{

}

TilemapSet::TilemapSet(
	const u8 paletteLength
	, const u16 * palette
	, const u32 tilesetLength
	, const u32 * tileset
	, const u32 tileSetDataCompressionFlags
	, const u8 mapCount
	, const u8 * mapTileWidths
	, const u8 * mapTileHeights
	, const u16 * mapData
)
{
	m_paletteLength = paletteLength;
	m_palette = palette;

	m_tileSetDataCompressionFlags = tileSetDataCompressionFlags;
	m_tilesetLength = tilesetLength;
	m_tileset = tileset;

	m_maps.Reserve(mapCount);

	int mapDataOffset = 0;
	for (u32 i = 0; i < mapCount; ++i)
	{
		Tilemap* tilemap = m_maps.AddNew();
		tilemap->m_tilemapSet = this;

		u8 widthInTiles = mapTileWidths[i];
		u8 heightInTiles = mapTileHeights[i];
		u16 totalTiles = widthInTiles * heightInTiles;

		tilemap->m_sizeInTiles = Vector2<u8>(widthInTiles, heightInTiles);

		tilemap->m_tileMapData = &mapData[mapDataOffset];
		tilemap->m_tileMapDataLength = totalTiles;
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
	return m_paletteIndex != INVALID_PALETTE_INDEX;
}

bool TilemapSet::IsTileSetLoaded() const
{
	return GetTileSetCharacterBaseBlock() != INVALID_TILESET_CBB;
}

bool TilemapSet::IsLoaded() const
{
	return IsPaletteLoaded() && IsTileSetLoaded();
}
