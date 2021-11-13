#pragma once

#include "engine/base/core/stl/List.h"
#include "engine/algorithm/Compression.h"
#include "engine/base/colour/Palette.h"
#include "GBASprite.h"

namespace GBA
{
	namespace Gfx
	{
		// Container for a list of sprites that are on the same atlus. 
		// Sprites on the same atlus will share a colour palette between them, reducing palette memory usage.
		class SpriteAtlus
		{
			friend class SpriteManager;

			const u16* m_palette = NULL;
			u8 m_paletteLength = 0;
			u32 m_spriteDataCompressionFlags = 0;
			List<Sprite> m_sprites;

			tPaletteIndex m_paletteIndex = INVALID_PALETTE_INDEX;

		public:
			SpriteAtlus();
			SpriteAtlus(
				const u32 spriteCount,
				const u8 paletteLength,
				const u16* palette,
				const u8* widthMap,
				const u8* heightMap,
				const u32 dataLength,
				const u32 compressionFlags,
				const u32* data,
				const u32* offsets);
			SpriteAtlus(const SpriteAtlus& that);

			SpriteAtlus & operator=(const SpriteAtlus& that);

			bool IsPaletteLoaded();
			inline tPaletteIndex GetPaletteIndex() { return m_paletteIndex; }

			inline u32 GetSpriteDataCompressionFlags() {
				return m_spriteDataCompressionFlags;
			}

			Sprite* GetSprite(int index) {
				if (index >= 0 && index < (int)m_sprites.Count())
					return &m_sprites[index];
				else
					return nullptr;
			}
		};
	}
}
