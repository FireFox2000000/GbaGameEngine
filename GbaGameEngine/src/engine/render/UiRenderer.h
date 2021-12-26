#pragma once
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"
#include <string>

class UiRenderer
{
public:
	struct FontProperties
	{
		// Tilemap index of the first ascii character, i.e. '!'
		int fontAsciiStart;

		// Max size in tiles of the font, for line break etc. 
		Vector2<int> fixedCharacterSize;
	};

private:
	GBA::Gfx::TilemapSet m_tilemapSet;
	GBA::BackgroundControl::Backgrounds m_backgroundId;
	GBA::tScreenBaseBlockIndex m_mapSbbIndex = GBA::INVALID_SBB_ID;
	u16 m_clearScreenEntry;
	FontProperties m_fontProperties;

	void Init();
	void AllocateMemory();
	void UnloadTilemapSet();

public:
	UiRenderer();
	~UiRenderer();

	void LoadAtlus(const u32* file, const FontProperties& fontProperties);
	void DrawUiElement(const Vector2<int>& screenPositionInTiles, int uiElementIndex) const;
	void ClearRegion(int x, int y, int width, int height) const;
	void RenderText(const std::string& str, const Vector2<int>& drawPosition) const;
};
