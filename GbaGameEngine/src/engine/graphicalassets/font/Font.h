#pragma once
#include "engine/graphicalassets/Graphics.h"

class Font
{
	friend class FontLibrary;

	SpriteAtlus m_spriteAtlus;
	int (*m_charToSpriteIndexLookupFn)(char);
	Vector2<u8> m_fixedCharacterSize;

public:
	Font(const SpriteAtlus& spriteAtlus, int(*charToSpriteIndexLookupFn)(char));

	const Vector2<u8> GetFixedCharacterSize() const;

	inline Sprite* GetSpriteForIndex(int index) { return m_spriteAtlus.GetSprite(index); }
	inline Sprite* GetSpriteForCharacter(char c) { return GetSpriteForIndex(m_charToSpriteIndexLookupFn(c)); }
};
