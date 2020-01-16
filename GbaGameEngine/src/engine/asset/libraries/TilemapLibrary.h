#pragma once
#include "engine/graphicalassets/Tilemap/TilemapSet.h"
#include "engine/base/core/stl/List.h"

#define TILEMAP_SET_LIST \
//

namespace TilemapSetID
{
#define TILEMAP_SET_ENTRY(Namespace) Namespace,
	enum Enum
	{
		TILEMAP_SET_LIST
		Count
	};
#undef TILEMAP_SET_ENTRY
}

class TilemapLibrary
{
	FixedList<TilemapSet, TilemapSetID::Count> m_tilemapSetCollection;

protected:
	void AddTilemapSet(
		const u8 paletteLength,
		const u16* palette,
		const u32 tilesetLength,
		const u32* tileset,
		const u32 compressionFlags,
		const u8 mapCount,
		const u8* widthMap,
		const u8* heightMap,
		const u16* mapData
		);

public:
	TilemapLibrary();
	~TilemapLibrary();

	Tilemap* GetTilemap(TilemapSetID::Enum tilemapSetId, u32 mapIndex);
};

////////////////////////////////////////////////////////////////////////////
/* Helpful marcros for registering tilemaps */

#define TILEMAPLIB_DEFINE_TILEMAP_EXTRENS(Prefix, Namespace) \
namespace Prefix##Namespace\
{\
	extern const u8 paletteLength; \
	extern const u16 palette[]; \
	extern const u32 tilesetLength; \
	extern const u32 tileset[]; \
	extern const u32 tileSetCompressionTypeSize; \
	extern const u8 mapCount;\
	extern const u8 mapTileWidths[]; \
	extern const u8 mapTileHeights[]; \
	extern const u16 map[]; \
}\

#define TILEMAPLIB_ADD_TILEMAP_SET(Prefix, Namespace) \
{\
	using namespace Prefix##Namespace;\
	AddTilemapSet(paletteLength, palette, tilesetLength, tileset, tileSetCompressionTypeSize, mapCount, mapTileWidths, mapTileHeights, map);\
	totalBytes += tilesetLength * sizeof(u32);\
	for (int i = 0; i < mapCount; ++i)\
	{\
		u32 mapSize = mapTileWidths[i] * mapTileHeights[i];\
		totalBytes += sizeof(u16) * mapSize;\
	}\
}
