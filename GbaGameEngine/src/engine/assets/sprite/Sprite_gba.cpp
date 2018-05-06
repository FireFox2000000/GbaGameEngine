#include "Sprite.h"
#include "engine\assets\sprite\SpriteAtlus.h"
#include "engine/base/colour/Palette.h"

tPaletteIndex Sprite::GetPaletteIndex() const
{
	return m_atlus ? m_atlus->GetPaletteIndex() : INVALID_PALETTE_INDEX;
}

bool Sprite::GetIsLoaded() const
{
	return m_tilesLoaded && m_atlus->IsPaletteLoaded();
}
