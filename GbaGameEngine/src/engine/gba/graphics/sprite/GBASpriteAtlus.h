#pragma once

#include "engine/base/core/stl/List.h"
#include "engine/algorithm/Compression.h"
#include "engine/base/colour/Palette.h"

class SpriteAssetManagerHelper;

namespace GBA
{
	namespace Gfx
	{
		struct SpriteNode;
		class Sprite;

		// Container for a list of sprites that are on the same atlus. 
		// Sprites on the same atlus will share a colour palette between them, reducing palette memory usage.
		class SpriteAtlus
		{
			friend class SpriteGraphicsMemoryManager;
			friend class ::SpriteAssetManagerHelper;

			const u16* m_palette;
			u8 m_paletteLength;
			u32 m_spriteDataCompressionFlags;
			SpriteNode* m_spritesLLHead;

			tPaletteIndex m_paletteIndex = INVALID_PALETTE_INDEX;

		public:
			SpriteAtlus();
			SpriteAtlus(
				const u8 paletteLength
				, const u16* palette
				, const u32 compressionFlags);
			SpriteAtlus(const SpriteAtlus& that);

			inline bool IsPaletteLoaded() { return GetPaletteIndex() != INVALID_PALETTE_INDEX; }
			inline tPaletteIndex GetPaletteIndex() { return m_paletteIndex; }

			inline u32 GetSpriteDataCompressionFlags() {
				return m_spriteDataCompressionFlags;
			}

			SpriteNode* GetHead() const { return m_spritesLLHead; }

			// This is slow, cache when possible
			Sprite* GetSprite(int index);

			// This is slow, cache when possible
			const Sprite* GetSprite(int index) const;
		};
	}
}
