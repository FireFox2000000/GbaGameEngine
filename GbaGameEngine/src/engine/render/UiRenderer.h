#pragma once
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"
#include "engine/io/MemoryMappedFileView.h"
#include "gbatek/Vram.h"
#include <string>

class UiRenderer
{
	struct FontProperties
	{
		// Tilemap index of the first ascii character, i.e. '!'
		int fontAsciiStart = 0;

		// The start of the supported ascii range. Generally ' ' or '!'
		char firstAsciiCharacter = 0;

		// Max size in tiles of the font, for line break etc. 
		Vector2i fixedCharacterSize;
	};

	GBA::Gfx::TilemapSet m_tilemapSet;
	int m_backgroundId = 0;
	GBA::VramAllocator::tScreenBaseBlockIndex m_mapSbbIndex = GBA::VramAllocator::INVALID_SBB_ID;
	GBATEK::BackgroundTilemapEntry m_clearScreenEntry;
	FontProperties m_fontProperties;

	void Init();
	void AllocateMemory();
	void UnloadTilemapSet();

public:
	UiRenderer();
	~UiRenderer();

	void LoadAtlas(const MemoryMappedFileView file);
	void DrawUiElement(const Vector2i& screenPositionInTiles, int uiElementIndex) const;
	void ClearRegion(int x, int y, int width, int height) const;
	void RenderText(const std::string& str, const Vector2i& drawPosition) const;
	void RenderText(const char* str, int start, int length, const Vector2i& drawPosition) const;
};
