#pragma once
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"
#include "gbatek/Vram.h"
#include <string>

class UiRenderer
{
	struct FontProperties
	{
		// Tilemap index of the first ascii character, i.e. '!'
		int fontAsciiStart;

		// The start of the supported ascii range. Generally ' ' or '!'
		char firstAsciiCharacter;

		// Max size in tiles of the font, for line break etc. 
		Vector2<int> fixedCharacterSize;
	};

	GBA::Gfx::TilemapSet m_tilemapSet;
	int m_backgroundId;
	GBA::VramAllocator::tScreenBaseBlockIndex m_mapSbbIndex = GBA::VramAllocator::INVALID_SBB_ID;
	GBATEK::BackgroundTilemapEntry m_clearScreenEntry;
	FontProperties m_fontProperties;

	void Init();
	void AllocateMemory();
	void UnloadTilemapSet();

public:
	UiRenderer();
	~UiRenderer();

	void LoadAtlus(const u32* file);
	void DrawUiElement(const Vector2<int>& screenPositionInTiles, int uiElementIndex) const;
	void ClearRegion(int x, int y, int width, int height) const;
	void RenderText(const std::string& str, const Vector2<int>& drawPosition) const;
	void RenderText(const char* str, int start, int length, const Vector2<int>& drawPosition) const;
};
