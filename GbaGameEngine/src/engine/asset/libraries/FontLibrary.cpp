#include "FontLibrary.h"
#include "engine/math/Math.h"
#include "engine/engine/engine.h"
#include "game/data/sprites/debug_font_8x8_bold.h"

FontLibrary::FontLibrary()
{
	DEBUG_LOG("Setting up Font Library...")

	DEBUG_LOG("Loading debug font bold sprite atlas");
	AddFontFromSpriteSheet(debug_font_8x8_bold::data, AsciiExclamationOffset);
}

FontLibrary::~FontLibrary()
{
	auto& spriteAssetManager = Engine::GetInstance().GetSpriteAssetManager();
	for (auto& font : m_fontCollection)
	{
		spriteAssetManager.UnloadSpriteAtlas(font.m_spriteAtlas);
	}
}

void FontLibrary::AddFontFromSpriteSheet(
	const u32* file
	, int(*charToSpriteIndexLookupFn)(char))
{
	auto* atlas = Engine::GetInstance().GetSpriteAssetManager().CreateSpriteAtlasFromFile(file);
	m_fontCollection.AddNew(atlas, charToSpriteIndexLookupFn);
}

Font * FontLibrary::GetFont(FontID::Enum fontId)
{
	return &m_fontCollection[fontId];
}
