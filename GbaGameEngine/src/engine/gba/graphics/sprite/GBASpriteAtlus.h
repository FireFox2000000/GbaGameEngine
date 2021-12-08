#pragma once
#include "engine/base/core/stl/List.h"
#include "engine/base/colour/Palette.h"

template<typename T>
class IPool;

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

			const u16* m_palette;
			u8 m_paletteLength;
			u32 m_spriteDataCompressionFlags;
			SpriteNode* m_spritesLLHead;

			tPaletteIndex m_paletteIndex = INVALID_PALETTE_INDEX;

		public:
			SpriteAtlus();
			static SpriteAtlus* CreateFromFile(const u32* file, IPool<SpriteAtlus>* spriteAtlusPool, IPool<SpriteNode>* spriteNodePool);

			bool IsPaletteLoaded();
			tPaletteIndex GetPaletteIndex();
			u32 GetSpriteDataCompressionFlags();

			SpriteNode* GetHead() const;

			// This is slow, cache when possible
			Sprite* GetSprite(int index);

			// This is slow, cache when possible
			const Sprite* GetSprite(int index) const;
		};
	}
}
