#pragma once
#include "engine/base/core/stl/List.h"
#include "engine/base/core/stl/Span.h"
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

			Span<const GBATEK::ColourRGB16> m_palette;
			u32 m_spriteDataCompressionFlags = 0;
			SpriteNode* m_spritesLLHead = nullptr;

			tPaletteIndex m_paletteIndex = INVALID_PALETTE_INDEX;
			int m_assetHash = -1;

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

			const int GetAssetHash() const { return m_assetHash; }
		};
	}
}
