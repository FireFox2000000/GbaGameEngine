#pragma once
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"

class Font
{
	friend class FontLibrary;

	GBA::Gfx::SpriteAtlus m_spriteAtlus;
	int (*m_charToSpriteIndexLookupFn)(char);
	Vector2<u8> m_fixedCharacterSize;

public:
	Font(const GBA::Gfx::SpriteAtlus& spriteAtlus, int(*charToSpriteIndexLookupFn)(char));

	const Vector2<u8> GetFixedCharacterSize() const;

	inline GBA::Gfx::Sprite* GetSpriteForIndex(int index) { return m_spriteAtlus.GetSprite(index); }
	inline GBA::Gfx::Sprite* GetSpriteForCharacter(char c) { return GetSpriteForIndex(m_charToSpriteIndexLookupFn(c)); }
};
