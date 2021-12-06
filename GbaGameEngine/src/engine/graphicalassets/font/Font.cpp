#include "Font.h"
#include "engine/gba/graphics/sprite/GBASprite.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"

Font::Font(GBA::Gfx::SpriteAtlus* spriteAtlus, int(*charToSpriteIndexLookupFn)(char))
	: m_spriteAtlus(spriteAtlus)
{
	m_charToSpriteIndexLookupFn = charToSpriteIndexLookupFn;

	const GBA::Gfx::Sprite* referenceSprite = m_spriteAtlus->GetSprite(0);

	DEBUG_ASSERTMSG(referenceSprite, "Fonts must contain at least one sprite!");

	Vector2<u8> fixedCharacterSize;
	auto pixelSize = referenceSprite->GetSizeInPixels();
	fixedCharacterSize.x = pixelSize.x;
	fixedCharacterSize.y = pixelSize.y;

	m_fixedCharacterSize = fixedCharacterSize;

	// Pre-cache sprites in a list to speed up text renderer. SpriteAtlus::GetSprite is 0n per character and would be way too slow.
	auto* spriteNode = m_spriteAtlus->GetHead();
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

GBA::Gfx::Sprite* Font::GetSpriteForIndex(int index)
{
	return m_spritesCache[index];
}

GBA::Gfx::Sprite* Font::GetSpriteForCharacter(char c)
{
	return GetSpriteForIndex(m_charToSpriteIndexLookupFn(c));
}
