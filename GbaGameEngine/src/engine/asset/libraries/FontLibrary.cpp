#include "FontLibrary.h"
#include "engine/math/Math.h"

#define FONT_NAMESPC_PREFIX __binary_spritesheet_

#define _FONT_ENTRY(Prefix, Namespace, LookupFn) FONTLIB_DEFINE_SPRITE_EXTRENS(Prefix, Namespace, LookupFn)
#define FONT_ENTRY(Namespace, LookupFn) _FONT_ENTRY(FONT_NAMESPC_PREFIX, Namespace, LookupFn)

	FONT_LIST
#undef FONTLIB_DEFINE_SPRITE_EXTRENS
#undef _FONT_ENTRY
#undef FONT_ENTRY

FontLibrary::FontLibrary()
{
	DEBUG_LOG("Setting up Font Library...")

	u32 totalBytes = 0;

#define _FONT_ENTRY(Prefix, Namespace, LookupFn) FONTLIB_ADD_SPRITE_SHEET(Prefix, Namespace, LookupFn)
#define FONT_ENTRY(Namespace, LookupFn) _FONT_ENTRY(FONT_NAMESPC_PREFIX, Namespace, LookupFn)

	FONT_LIST

#undef ADD_SPRITE_SHEET
#undef _FONT_ENTRY
#undef FONT_ENTRY

	DEBUG_LOGFORMAT("Font Library total sprite memory = %.2fkb", BYTES_TO_KB(totalBytes));
}

FontLibrary::~FontLibrary()
{
}

void FontLibrary::AddFontFromSpriteSheet(int(*charToSpriteIndexLookupFn)(char), const u32 spriteCount, const u8 paletteLength, const u16 * palette, const u8 * widthMap, const u8 * heightMap, const u32 dataLength, const u32 compressionFlags, const u32 * data, const u32 * offsets)
{
	GBA::Gfx::SpriteAtlus atlus(spriteCount, paletteLength, palette, widthMap, heightMap, dataLength, compressionFlags, data, offsets);
	m_fontCollection.AddNew(atlus, charToSpriteIndexLookupFn);
}

Font * FontLibrary::GetFont(FontID::Enum fontId)
{
	return &m_fontCollection[fontId];
}
