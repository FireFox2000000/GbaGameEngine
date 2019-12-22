#include "SpriteLibrary.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"
#include "engine/math/Math.h"

#define SPRITEMAP_NAMESPC_PREFIX __binary_spritesheet_

#define _SPRITE_ATLUS_ENTRY(Prefix, Namespace) SPRITELIB_DEFINE_SPRITE_EXTRENS(Prefix, Namespace)
#define SPRITE_ATLUS_ENTRY(Namespace) _SPRITE_ATLUS_ENTRY(SPRITEMAP_NAMESPC_PREFIX, Namespace)

	SPRITE_ATLUS_LIST
#undef DEFINE_SPRITE_ATLUS_EXTRENS
#undef _SPRITE_ATLUS_ENTRY
#undef SPRITE_ATLUS_ENTRY

SpriteLibrary::SpriteLibrary()
{
	DEBUG_LOG("Setting up Sprite Library...")

	u32 totalBytes = 0;

#define _SPRITE_ATLUS_ENTRY(Prefix, Namespace) SPRITELIB_ADD_SPRITE_SHEET(Prefix, Namespace)
#define SPRITE_ATLUS_ENTRY(Namespace) _SPRITE_ATLUS_ENTRY(SPRITEMAP_NAMESPC_PREFIX, Namespace)

	SPRITE_ATLUS_LIST

#undef ADD_SPRITE_SHEET
#undef _SPRITE_ATLUS_ENTRY
#undef SPRITE_ATLUS_ENTRY

	DEBUG_LOGFORMAT("Sprite Library total sprite memory = %.2fkb", BYTES_TO_KB(totalBytes));
}

void SpriteLibrary::AddSpriteSheet(
	const u32 spriteCount, 
	const u8 paletteLength, 
	const u16 * palette, 
	const u8 * widthMap, 
	const u8 * heightMap, 
	const u32 dataLength, 
	const u32 * data,
	const u32 * offsets)
{
	SpriteAtlus* atlus = m_spriteAtlusCollection.AddNew();
	atlus->m_paletteLength = paletteLength;
	atlus->m_palette = palette;

	atlus->m_sprites.Reserve(spriteCount);

	for (u32 i = 0; i < spriteCount; ++i)
	{
		Sprite* sprite = atlus->m_sprites.AddNew();
		sprite->m_atlus = atlus;
		GBA::AttributeFunctions::GetSizeAttributesFromPixelSize(GBAAttrFnVector2(widthMap[i], heightMap[i]), sprite->m_shape, sprite->m_sizeMode);
		sprite->m_pixelMapData = data + offsets[i];
		if (i + 1 < spriteCount)
		{
			sprite->m_pixelMapDataLength = offsets[i + 1] - offsets[i];
		}
		else
		{
			sprite->m_pixelMapDataLength = dataLength - offsets[i];
		}
	}
}

SpriteLibrary::~SpriteLibrary()
{
}

Sprite * SpriteLibrary::GetSprite(SpriteAtlusID::Enum atlusId, u32 spriteIndex)
{
	SpriteAtlus& atlus = m_spriteAtlusCollection[atlusId];
	if (spriteIndex < atlus.m_sprites.Count())
		return &atlus.m_sprites[spriteIndex];

	DEBUG_ASSERTMSGFORMAT(false, "Unable to get sprite for atlus %d at index %d", atlusId, spriteIndex);
	return NULL;
}
