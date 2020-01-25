#pragma once
#include "engine/gba/graphics/background/GBABackground.h"

class TilemapSet;

class Tilemap
{
public:

	friend class TilemapSet;

	TilemapSet* m_tilemapSet = NULL;
	TilemapSet* EditTilemapSet() { return m_tilemapSet; }

	const u16* m_tileMapData = NULL;
	u16 m_tileMapDataLength = 0;

	Vector2<u8> m_sizeInTiles;

	GBA::tScreenBaseBlockIndex m_mapSbbIndex = GBA::INVALID_SBB_ID;

//public:
	Tilemap();

	GBA::Background::ControlRegister::Size GetSize() const;
	const Vector2<u8>& GetSizeInTiles() const { return m_sizeInTiles; }
	inline GBA::tScreenBaseBlockIndex GetMapScreenBaseBlockIndex() const { return m_mapSbbIndex; }
	bool IsLoaded() const;
};
