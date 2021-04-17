#include "TilemapLibrary.h"
#include "engine/math/Math.h"

#define TILEMAP_SET_NAMESPC_PREFIX __binary_background_

#define _TILEMAP_SET_ENTRY(Prefix, Namespace) TILEMAPLIB_DEFINE_TILEMAP_EXTRENS(Prefix, Namespace)
#define TILEMAP_SET_ENTRY(Namespace) _TILEMAP_SET_ENTRY(TILEMAP_SET_NAMESPC_PREFIX, Namespace)

	TILEMAP_SET_LIST
#undef DEFINE_TILEMAP_SET_EXTRENS
#undef _TILEMAP_SET_ENTRY
#undef TILEMAP_SET_ENTRY

TilemapLibrary::TilemapLibrary()
{
	DEBUG_LOG("Setting up Tilemap Library...")

		u32 totalBytes = 0;

#define _TILEMAP_SET_ENTRY(Prefix, Namespace) TILEMAPLIB_ADD_TILEMAP_SET(Prefix, Namespace)
#define TILEMAP_SET_ENTRY(Namespace) _TILEMAP_SET_ENTRY(TILEMAP_SET_NAMESPC_PREFIX, Namespace)

	TILEMAP_SET_LIST

#undef TILEMAPLIB_ADD_TILEMAP_SET
#undef _TILEMAP_SET_ENTRY
#undef TILEMAP_SET_ENTRY

		DEBUG_LOGFORMAT("Tilemap Library total tilemap memory = %.2fkb", BYTES_TO_KB(totalBytes));
}

TilemapLibrary::~TilemapLibrary()
{
}

void TilemapLibrary::AddTilemapSet(
	const u8 paletteLength
	, const u16 * palette
	, const u32 tilesetLength
	, const u32 * tileset
	, const u32 compressionFlags
	, const u8 mapCount
	, const u8 mapIsDynamicMask
	, const u8 * widthMap
	, const u8 * heightMap
	, const u16 * mapData
)
{
	m_tilemapSetCollection.AddNew(paletteLength, palette, tilesetLength, tileset, compressionFlags, mapCount, mapIsDynamicMask, widthMap, heightMap, mapData);
}

Tilemap * TilemapLibrary::GetTilemap(TilemapSetID::Enum tilemapSetId, u32 mapIndex)
{
	TilemapSet& tilemapSet = m_tilemapSetCollection[tilemapSetId];
	Tilemap* tilemap = tilemapSet.GetTilemap(mapIndex);

	DEBUG_ASSERTMSGFORMAT(tilemap, "Unable to get tilemap for tilemap set %d at index %d", tilemapSetId, mapIndex);

	return tilemap;
}
