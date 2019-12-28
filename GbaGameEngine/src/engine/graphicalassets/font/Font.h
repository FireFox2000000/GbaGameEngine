#pragma once
#include "engine/graphicalassets/sprite/SpriteAtlus.h"

class Font
{
	friend class FontLibrary;

	SpriteAtlus m_spriteAtlus;
	int (*m_charToSpriteIndexLookupFn)(char);

public:
	Font(const SpriteAtlus& spriteAtlus, int(*charToSpriteIndexLookupFn)(char));

	inline Sprite* GetSpriteForIndex(int index) { return m_spriteAtlus.GetSprite(index); }
	inline Sprite* GetSpriteForCharacter(char c) { return GetSpriteForIndex(m_charToSpriteIndexLookupFn(c)); }
};
