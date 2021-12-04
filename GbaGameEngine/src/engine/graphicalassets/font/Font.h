#pragma once
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/List.h"

namespace GBA
{
	namespace Gfx
	{
		class Sprite;
	}
}

class Font
{
	friend class FontLibrary;

	GBA::Gfx::SpriteAtlus* m_spriteAtlus;
	int (*m_charToSpriteIndexLookupFn)(char);
	Vector2<u8> m_fixedCharacterSize;
	List<GBA::Gfx::Sprite*> m_spritesCache;

public:
	Font(GBA::Gfx::SpriteAtlus* spriteAtlus, int(*charToSpriteIndexLookupFn)(char));

	const Vector2<u8> GetFixedCharacterSize() const;

	inline GBA::Gfx::Sprite* GetSpriteForIndex(int index) { return m_spritesCache[index]; }
	inline GBA::Gfx::Sprite* GetSpriteForCharacter(char c) { return GetSpriteForIndex(m_charToSpriteIndexLookupFn(c)); }
};
