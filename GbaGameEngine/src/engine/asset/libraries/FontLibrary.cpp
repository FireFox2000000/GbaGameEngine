#include "FontLibrary.h"
#include "engine/math/Math.h"
#include "game/data/sprites/debug_font_8x8_bold.h"

FontLibrary::FontLibrary()
{
	DEBUG_LOG("Setting up Font Library...")

	DEBUG_LOG("Loading debug font bold sprite atlus");
	AddFontFromSpriteSheet(debug_font_8x8_bold::data, AsciiExclamationOffset);
}

FontLibrary::~FontLibrary()
{
}

void FontLibrary::AddFontFromSpriteSheet(
	const u32* file
	, int(*charToSpriteIndexLookupFn)(char))
{
	auto* atlus = m_spriteAssetManager.CreateSpriteAtlusFromFile(file);
	m_fontCollection.AddNew(atlus, charToSpriteIndexLookupFn);
}

Font * FontLibrary::GetFont(FontID::Enum fontId)
{
	return &m_fontCollection[fontId];
}
