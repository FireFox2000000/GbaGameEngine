#include "SpriteAtlus.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"

SpriteAtlus::SpriteAtlus()
{
}

SpriteAtlus::SpriteAtlus(const u32 spriteCount, const u8 paletteLength, const u16 * palette, const u8 * widthMap, const u8 * heightMap, const u32 dataLength, const u32 compressionFlags, const u32 * data, const u32 * offsets)
{
	m_paletteLength = paletteLength;
	m_palette = palette;
	m_spriteDataCompressionFlags = compressionFlags;

	m_sprites.Reserve(spriteCount);

	for (u32 i = 0; i < spriteCount; ++i)
	{
		Sprite* sprite = m_sprites.AddNew();
		sprite->m_atlus = this;
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

SpriteAtlus::SpriteAtlus(const SpriteAtlus & that)
{
	*this = that;
	for (Sprite& sprite : m_sprites)
	{
		sprite.m_atlus = this;
	}
}

SpriteAtlus::~SpriteAtlus()
{
}

bool SpriteAtlus::IsPaletteLoaded()
{
	return GetPaletteIndex() != INVALID_PALETTE_INDEX;
}
