#include "Font.h"
#include "engine/graphics/sprite/Sprite.h"
#include "engine/graphics/sprite/SpriteAtlas.h"
#include "engine/graphics/sprite/SpriteNode.h"

Font::Font(SpriteAtlas* spriteAtlas, int(*charToSpriteIndexLookupFn)(char))
	: m_spriteAtlas(spriteAtlas)
{
	m_charToSpriteIndexLookupFn = charToSpriteIndexLookupFn;

	const Sprite* referenceSprite = m_spriteAtlas->GetSprite(0);

	DEBUG_ASSERTMSG(referenceSprite, "Fonts must contain at least one sprite!");

	Vector2<u8> fixedCharacterSize;
	auto pixelSize = referenceSprite->GetSizeInPixels();
	fixedCharacterSize.x = pixelSize.x;
	fixedCharacterSize.y = pixelSize.y;

	m_fixedCharacterSize = fixedCharacterSize;

	// Pre-cache sprites in a list to speed up text renderer. SpriteAtlas::GetSprite is 0n per character and would be way too slow.
	auto* spriteNode = m_spriteAtlas->GetHead();
	while (spriteNode)
	{
		m_spritesCache.Add(&spriteNode->sprite);
		spriteNode = spriteNode->next;
	}
}

const Vector2<u8> Font::GetFixedCharacterSize() const
{
	return m_fixedCharacterSize;
}

Sprite* Font::GetSpriteForIndex(int index)
{
	return m_spritesCache[index];
}

Sprite* Font::GetSpriteForCharacter(char c)
{
	return GetSpriteForIndex(m_charToSpriteIndexLookupFn(c));
}
