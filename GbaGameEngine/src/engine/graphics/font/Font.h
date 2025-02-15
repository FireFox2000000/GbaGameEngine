#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/List.h"

class Sprite;
class SpriteAtlas;

class Font
{
	friend class FontLibrary;

	SpriteAtlas* m_spriteAtlas;
	int (*m_charToSpriteIndexLookupFn)(char);
	Vector2<u8> m_fixedCharacterSize;
	List<Sprite*> m_spritesCache;

public:
	Font(SpriteAtlas* spriteAtlas, int(*charToSpriteIndexLookupFn)(char));

	const Vector2<u8> GetFixedCharacterSize() const;

	Sprite* GetSpriteForIndex(int index);
	Sprite* GetSpriteForCharacter(char c);
};
