#pragma once
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"

class UiRenderer
{
	GBA::Gfx::TilemapSet m_tilemapSet;
	GBA::BackgroundControl::Backgrounds m_backgroundId;
	GBA::tScreenBaseBlockIndex m_mapSbbIndex = GBA::INVALID_SBB_ID;

	void Init();
	void AllocateMemory();
	void UnloadTilemapSet();

public:
	UiRenderer();
	~UiRenderer();

	void LoadAtlus(const u32* file);
	void DrawUiElement(const Vector2<int>& screenPositionInTiles, int uiElementIndex);
};
