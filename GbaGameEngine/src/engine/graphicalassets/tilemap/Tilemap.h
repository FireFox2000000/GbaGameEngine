#pragma once
#include "engine/gba/graphics/background/GBABackground.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"

class TilemapSet;

class Tilemap
{
	friend class TilemapSet;
	friend class TilemapManager;

	TilemapSet* m_tilemapSet = NULL;
	TilemapSet* EditTilemapSet() { return m_tilemapSet; }

	const u16* m_tileMapData = NULL;
	u16 m_tileMapDataLength = 0;

	Vector2<u8> m_sizeInTiles;
	GBA::Gfx::Background::ControlRegister::Size GetSize() const;

	// Runtime assigned render data when loaded
	GBA::tScreenBaseBlockIndex m_mapSbbIndex = GBA::INVALID_SBB_ID;
	GBA::BackgroundControl::Backgrounds m_backgroundSlotId = GBA::BackgroundControl::Count;

public:
	Tilemap();

	bool IsLoaded() const;
	inline const Vector2<u8>& GetSizeInTiles() const { return m_sizeInTiles; }
	inline GBA::tScreenBaseBlockIndex GetMapScreenBaseBlockIndex() const { return m_mapSbbIndex; }
	inline const TilemapSet* GetTilemapSet() const { return m_tilemapSet; }
	inline u16 GetTileMapLength() const { return m_tileMapDataLength; }
	inline const u16* GetTileMapData() const { return m_tileMapData; }
	inline GBA::BackgroundControl::Backgrounds GetAssignedBackgroundSlot() const { return m_backgroundSlotId; }
};
