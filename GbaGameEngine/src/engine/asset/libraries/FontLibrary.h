#pragma once
#include "engine/graphicalassets/font/Font.h"
#include "engine/graphicalassets/font/FontLookupFunctions.h"
#include "engine/base/core/stl/List.h"
#include "engine/asset/SpriteAssetManager.h"

namespace FontID
{
	enum Enum
	{
		debug_font_8x8_bold,

		Count
	};
}

class FontLibrary
{
	SpriteAssetManager<1, 100> m_spriteAssetManager;
	FixedList<Font, FontID::Count> m_fontCollection;

protected:
	void AddFontFromSpriteSheet(
		const u32* file,
		int(*charToSpriteIndexLookupFn)(char)
		);

public:
	FontLibrary();
	~FontLibrary();

	Font* GetFont(FontID::Enum fontId);
};
