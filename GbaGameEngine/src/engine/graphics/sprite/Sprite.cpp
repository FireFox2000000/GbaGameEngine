#include "Sprite.h"
#include "SpriteAtlas.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"

SpriteAtlas* Sprite::EditAtlas()
{
	return m_atlas;
}

Sprite::~Sprite()
{
	DEBUG_ASSERTMSG(!IsLoaded(), "Sprite was destroyed while it was still loaded in video memory!");
}

const SpriteAtlas* Sprite::GetAtlas() const
{
	return m_atlas;
}

Vector2i Sprite::GetSize() const
{
	return Vector2i(m_tileSize);
}

Vector2i Sprite::GetSizeInPixels() const
{
	return Vector2i(GBA::Gfx::AttributeFunctions::GetPixelSize(m_tileSize));
}

#ifdef Platform_GBA
bool Sprite::IsLoaded() const
{
	return m_gfxData.GetTileIndex() != GBA::Gfx::INVALID_TILE_ID && m_atlas->GetGfxData().IsPaletteLoaded();
}
#endif
