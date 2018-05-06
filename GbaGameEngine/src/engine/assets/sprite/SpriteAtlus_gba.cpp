#include "SpriteAtlus.h"
#include "engine/base/colour/Palette.h"

SpriteAtlus::SpriteAtlus()
	: m_paletteLength(0)
	, m_palette(NULL)
	, m_isPaletteLoaded(false)
	, m_paletteIndex(INVALID_PALETTE_INDEX)
{
}


SpriteAtlus::~SpriteAtlus()
{
}
