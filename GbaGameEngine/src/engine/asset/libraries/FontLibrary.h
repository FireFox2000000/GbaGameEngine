#pragma once
#include "engine/graphics/font/Font.h"
#include "engine/graphics/font/FontLookupFunctions.h"
#include "engine/base/core/stl/List.h"

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
