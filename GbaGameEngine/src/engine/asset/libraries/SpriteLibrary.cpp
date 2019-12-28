#include "SpriteLibrary.h"
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
	const u32 compressionFlags,
	const u32 * data,
	const u32 * offsets)
{
	m_spriteAtlusCollection.AddNew(spriteCount, paletteLength, palette, widthMap, heightMap, dataLength, compressionFlags, data, offsets);
}

SpriteLibrary::~SpriteLibrary()
{
}

Sprite * SpriteLibrary::GetSprite(SpriteAtlusID::Enum atlusId, u32 spriteIndex)
{
	SpriteAtlus& atlus = m_spriteAtlusCollection[atlusId];
	Sprite* sprite = atlus.GetSprite(spriteIndex);

	DEBUG_ASSERTMSGFORMAT(sprite, "Unable to get sprite for atlus %d at index %d", atlusId, spriteIndex);

	return sprite;
}
