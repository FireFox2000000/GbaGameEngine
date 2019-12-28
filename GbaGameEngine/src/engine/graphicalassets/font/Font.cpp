#include "Font.h"

Font::Font(const SpriteAtlus & spriteAtlus, int(*charToSpriteIndexLookupFn)(char))
	: m_spriteAtlus(spriteAtlus)
{
	m_charToSpriteIndexLookupFn = charToSpriteIndexLookupFn;
}
