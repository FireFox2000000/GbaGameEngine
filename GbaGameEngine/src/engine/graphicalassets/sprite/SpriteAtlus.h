#pragma once

#include "engine/base/core/stl/List.h"
#include "engine/graphicalassets/sprite/Sprite.h"
#include "engine/algorithm/Compression.h"

class SpriteAtlus
{
	friend class SpriteManager;
	friend class SpriteLibrary;

	const u16* m_palette;
	u8 m_paletteLength;
	u32 m_spriteDataCompressionFlags = 0;
	List<Sprite> m_sprites;

	tPaletteIndex m_paletteIndex;

public:
	SpriteAtlus();
	~SpriteAtlus();

	bool IsPaletteLoaded();
	inline tPaletteIndex GetPaletteIndex() { return m_paletteIndex; }

	inline u32 GetSpriteDataCompressionFlags() {
		return m_spriteDataCompressionFlags;
	}
};
