#pragma once

#include "engine/graphicalassets/sprite/SpriteAtlus.h"
#include "engine/base/core/stl/List.h"

#define SPRITE_ATLUS_LIST \
	SPRITE_ATLUS_ENTRY(Player)\
	SPRITE_ATLUS_ENTRY(Reimu)\
//

namespace SpriteAtlusID
{
#define SPRITE_ATLUS_ENTRY(Namespace) Namespace,
	enum Enum
	{
		SPRITE_ATLUS_LIST
		Count
	};
#undef SPRITE_ATLUS_ENTRY
}

class SpriteLibrary
{
	FixedList<SpriteAtlus, SpriteAtlusID::Count> m_spriteAtlusCollection;

protected:
	void AddSpriteSheet(
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
	SpriteLibrary();
	~SpriteLibrary();

	Sprite* GetSprite(SpriteAtlusID::Enum atlusId, u32 spriteIndex);
};

////////////////////////////////////////////////////////////////////////////
/* Helpful marcros for registering sprites */

#define SPRITELIB_DEFINE_SPRITE_EXTRENS(Prefix, Namespace) \
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

#define SPRITELIB_ADD_SPRITE_SHEET(Prefix, Namespace) \
{\
	using namespace Prefix##Namespace;\
	AddSpriteSheet(spriteCount, paletteLength, palette, widthMap, heightMap, dataLength, compressionTypeSize, data, offsets);\
	totalBytes += sizeof(u32) * dataLength;\
}
