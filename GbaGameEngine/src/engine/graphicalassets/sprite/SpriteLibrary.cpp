#include "SpriteLibrary.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"

#define SPRITE_ATLUS_ENTRY(Namespace) \
namespace Namespace\
{\
	extern const u32 spriteCount; \
	extern const u8 paletteLength; \
	extern const u16 palette[]; \
	extern const u8 widthMap[]; \
	extern const u8 heightMap[]; \
	extern const u32 dataLength; \
	extern const u16 data[]; \
	extern const u32 offsets[]; \
}\

	SPRITE_ATLUS_LIST
#undef SPRITE_ATLUS_ENTRY

SpriteLibrary::SpriteLibrary()
{
#define SPRITE_ATLUS_ENTRY(Namespace) \
	{\
		SpriteAtlus* atlus = m_spriteAtlusCollection.AddNew();\
		\
		atlus->m_paletteLength = Namespace::paletteLength;\
		atlus->m_palette = Namespace::palette;\
		\
		atlus->m_sprites.Reserve(Namespace::spriteCount);\
		\
		for (u32 i = 0; i < Namespace::spriteCount; ++i)\
		{\
			Sprite* sprite = atlus->m_sprites.AddNew();\
			sprite->m_atlus = atlus;\
			GBA::AttributeFunctions::GetSizeAttributesFromPixelSize(GBAAttrFnVector2(Namespace::widthMap[i], Namespace::heightMap[i]), sprite->m_shape, sprite->m_sizeMode);\
			sprite->m_pixelMapData = Namespace::data + Namespace::offsets[i];\
			if (i + 1 < Namespace::spriteCount)\
				sprite->m_pixelMapDataLength = Namespace::offsets[i + 1] - Namespace::offsets[i];\
			else\
				sprite->m_pixelMapDataLength = Namespace::dataLength - Namespace::offsets[i];\
		}\
	}

	SPRITE_ATLUS_LIST

#undef SPRITE_ATLUS_ENTRY
}


SpriteLibrary::~SpriteLibrary()
{
}

Sprite * SpriteLibrary::GetSprite(SpriteAtlusID::Enum atlusId, u32 spriteIndex)
{
	SpriteAtlus& atlus = m_spriteAtlusCollection[atlusId];
	if (spriteIndex < atlus.m_sprites.Count())
		return &atlus.m_sprites[spriteIndex];

	return NULL;
}
