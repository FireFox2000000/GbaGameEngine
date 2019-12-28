#pragma once
#include "engine/graphicalassets/font/Font.h"
#include "engine/graphicalassets/font/FontLookupFunctions.h"
#include "engine/base/core/stl/List.h"

#define FONT_LIST \
	FONT_ENTRY(debug_font_8x8, AsciiExclamationOffset)\
//

namespace FontID
{
#define FONT_ENTRY(Namespace, LookupFn) Namespace,
	enum Enum
	{
		FONT_LIST
		Count
	};
#undef FONT_ENTRY
}

class FontLibrary
{
	FixedList<Font, FontID::Count> m_fontCollection;

protected:
	void AddFontFromSpriteSheet(
		int(*charToSpriteIndexLookupFn)(char),
		const u32 spriteCount,
		const u8 paletteLength,
		const u16* palette,
		const u8* widthMap,
		const u8* heightMap,
		const u32 dataLength,
		const u32 compressionFlags,
		const u32* data,
		const u32* offsets);

public:
	FontLibrary();
	~FontLibrary();

	Font* GetFont(FontID::Enum fontId);
};

#define FONTLIB_DEFINE_SPRITE_EXTRENS(Prefix, Namespace, LookupFn) \
namespace Prefix##Namespace\
{\
	extern const u32 spriteCount; \
	extern const u8 paletteLength; \
	extern const u16 palette[]; \
	extern const u8 widthMap[]; \
	extern const u8 heightMap[]; \
	extern const u32 dataLength; \
	extern const u32 compressionTypeSize;\
	extern const u32 data[]; \
	extern const u32 offsets[]; \
}\

#define FONTLIB_ADD_SPRITE_SHEET(Prefix, Namespace, LookupFn) \
{\
	using namespace Prefix##Namespace;\
	AddFontFromSpriteSheet(LookupFn, spriteCount, paletteLength, palette, widthMap, heightMap, dataLength, compressionTypeSize, data, offsets);\
	totalBytes += sizeof(u32) * dataLength;\
}
