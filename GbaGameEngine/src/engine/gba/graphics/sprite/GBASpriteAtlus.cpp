#include "GBASpriteAtlus.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"

namespace GBA
{
	namespace Gfx
	{
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
				Attributes::Shape shape;
				Attributes::SizeMode sizeMode;
				AttributeFunctions::GetSizeAttributesFromPixelSize(GBAAttrFnVector2(widthMap[i], heightMap[i]), shape, sizeMode);

				Sprite* sprite = m_sprites.AddNew(shape, sizeMode);
				sprite->m_atlus = this;
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
		}

		SpriteAtlus & SpriteAtlus::operator=(const SpriteAtlus & that)
		{
			m_palette = that.m_palette;
			m_paletteLength = that.m_paletteLength;
			m_spriteDataCompressionFlags = that.m_spriteDataCompressionFlags;
			m_sprites = that.m_sprites;

			for (Sprite& sprite : m_sprites)
			{
				sprite.m_atlus = this;
			}

			return *this;
		}
	}
}
