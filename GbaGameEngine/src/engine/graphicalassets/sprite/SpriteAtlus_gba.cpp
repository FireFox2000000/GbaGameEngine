#include "SpriteAtlus.h"
#include "engine/base/colour/Palette.h"

SpriteAtlus::SpriteAtlus()
	: m_palette(NULL)
	, m_paletteLength(0)
	, m_paletteIndex(INVALID_PALETTE_INDEX)
{
}

SpriteAtlus::~SpriteAtlus()
{
}

bool SpriteAtlus::IsPaletteLoaded()
{
	return GetPaletteIndex() != INVALID_PALETTE_INDEX;
}
