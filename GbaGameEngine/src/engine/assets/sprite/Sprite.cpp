#include "Sprite.h"
#include "engine\assets\sprite\SpriteAtlus.h"

GBA::tPaletteBlockId Sprite::GetPaletteIndex() const
{
	return m_atlus ? m_atlus->GetPaletteIndex() : 16;
}

bool Sprite::GetIsLoaded() const
{
	return m_tilesLoaded && m_atlus->IsPaletteLoaded();
}
