#include "Sprite.h"
#include "engine/graphicalassets/sprite/SpriteAtlus.h"
#include "engine/base/colour/Palette.h"

Sprite::Sprite()
	: m_shape(GBA::Attributes::Square)
	, m_sizeMode(GBA::Attributes::Form0)
	, m_tileIndex(INVALID_TILE_ID)
	, m_atlus(NULL)
	, m_pixelMapData(NULL)
	, m_pixelMapDataLength(0)
{}

tPaletteIndex Sprite::GetPaletteIndex() const
{
	return m_atlus ? m_atlus->GetPaletteIndex() : INVALID_PALETTE_INDEX;
}

bool Sprite::IsLoaded() const
{
	return GetTileIndex() != INVALID_TILE_ID && m_atlus->IsPaletteLoaded();
}
