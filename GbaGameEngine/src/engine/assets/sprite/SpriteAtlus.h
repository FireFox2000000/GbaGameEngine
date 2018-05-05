#ifndef PRAGMA_ONCE_ENGINE_ASSETS_SPRITE_SPRITEATLUS_H
#define PRAGMA_ONCE_ENGINE_ASSETS_SPRITE_SPRITEATLUS_H

#include "engine/base/core/stl/List.h"
#include "engine/assets/sprite/Sprite.h"

class SpriteAtlus
{
	friend class SpriteLoader;
	friend class SpriteLibrary;

	u8 m_paletteLength;
	const u16* m_palette;
	List<Sprite> m_sprites;

	bool m_isPaletteLoaded;
	u8 m_paletteIndex;

public:
	SpriteAtlus();
	~SpriteAtlus();

	inline bool IsPaletteLoaded() { return m_isPaletteLoaded; }
	inline u8 GetPaletteIndex() { return m_paletteIndex; }
};

#endif
