#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/List.h"

namespace GBA
{
	namespace Gfx
	{
		class Sprite;
		class SpriteAtlas;
	}
}

class Font
{
	friend class FontLibrary;

	GBA::Gfx::SpriteAtlas* m_spriteAtlas;
	int (*m_charToSpriteIndexLookupFn)(char);
	Vector2<u8> m_fixedCharacterSize;
	List<GBA::Gfx::Sprite*> m_spritesCache;

public:
	Font(GBA::Gfx::SpriteAtlas* spriteAtlas, int(*charToSpriteIndexLookupFn)(char));

	const Vector2<u8> GetFixedCharacterSize() const;

	GBA::Gfx::Sprite* GetSpriteForIndex(int index);
	GBA::Gfx::Sprite* GetSpriteForCharacter(char c);
};
