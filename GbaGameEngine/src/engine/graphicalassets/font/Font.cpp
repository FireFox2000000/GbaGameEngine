#include "Font.h"

Font::Font(const SpriteAtlus & spriteAtlus, int(*charToSpriteIndexLookupFn)(char))
	: m_spriteAtlus(spriteAtlus)
{
	m_charToSpriteIndexLookupFn = charToSpriteIndexLookupFn;

	Sprite* referenceSprite = m_spriteAtlus.GetSprite(0);

	DEBUG_ASSERTMSG(referenceSprite, "Fonts must contain at least one sprite!");

	Vector2<u8> fixedCharacterSize;
	auto pixelSize = referenceSprite->GetSizeInPixels();
	fixedCharacterSize.x = pixelSize.x;
	fixedCharacterSize.y = pixelSize.y;

	m_fixedCharacterSize = fixedCharacterSize;
}

const Vector2<u8> Font::GetFixedCharacterSize() const
{
	return m_fixedCharacterSize;
}
