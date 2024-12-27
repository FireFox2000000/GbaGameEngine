#include "GBASpriteGraphicsMemoryManager.h"
#include "GBASprite.h"
#include "GBASpriteAtlas.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVramAllocator.h"

namespace GBA
{
	namespace Gfx
	{
		SpriteGraphicsMemoryManager::SpriteGraphicsMemoryManager()
			: m_paletteRefTracker(0)
		{
		}

		SpriteGraphicsMemoryManager::~SpriteGraphicsMemoryManager()
		{
		}

		void SpriteGraphicsMemoryManager::Load(Sprite& out_sprite)
		{
			using namespace GBA;

			if (out_sprite.IsLoaded())
				return;

			// Load palette
			SpriteAtlas* atlas = out_sprite.EditAtlas();
			tPaletteIndex paletteId = 0;

			if (!out_sprite.m_atlas->IsPaletteLoaded())
			{
				// Find the next available palette slot
				while (paletteId < m_paletteRefTracker.Count() && m_paletteRefTracker[paletteId] > 0)
				{
					++paletteId;
				}

				if (paletteId >= m_paletteRefTracker.Count())
				{
					DEBUG_ERROR("Unable to load sprite as there is no available palette space remaining.");
					return;		// Out of palette space!
				}

				ColourPalette16 palette(GBATEK::ColourRGB16{ 0, 0, 0 });

				for (u32 i = 0; i < atlas->m_palette.Count(); ++i)
				{
					palette[i] = atlas->m_palette[i];
				}
				PaletteBank::LoadSpritePalette(paletteId, palette);
				atlas->m_paletteIndex = paletteId;
			}
			else
			{
				paletteId = out_sprite.GetPaletteIndex();
			}

			++m_paletteRefTracker[paletteId];

			u32 compressionFlags = out_sprite.m_atlas->GetSpriteDataCompressionFlags();
			tTileId tileIndex = GBA::VramAllocator::GetInstance().AllocSpriteMem(out_sprite.m_pixelMapData.Data(), out_sprite.m_pixelMapData.Count(), compressionFlags);

			if (tileIndex != INVALID_TILE_ID)
			{
				// Set sprite attributes
				out_sprite.m_renderData.SetTileIndex(tileIndex);
			}
			else
			{
				DEBUG_ERROR("Unable to load sprite as there is no available tile space remaining.");

				// Undo the palette we just loaded
				--m_paletteRefTracker[paletteId];
			}
		}

		void SpriteGraphicsMemoryManager::Unload(Sprite * sprite)
		{
			if (!sprite->IsLoaded())
				return;

			// Remove tile references
			tTileId index = sprite->m_renderData.GetTileIndex();
			GBA::VramAllocator::GetInstance().FreeSpriteMem(index);
			sprite->m_renderData.SetTileIndex(INVALID_TILE_ID);

			// Decrease palette references
			--m_paletteRefTracker[sprite->GetPaletteIndex()];
			if (m_paletteRefTracker[sprite->GetPaletteIndex()] <= 0)
			{
				// Free it so it can be reused
				SpriteAtlas* atlas = sprite->m_atlas;
				atlas->m_paletteIndex = INVALID_PALETTE_INDEX;
			}
		}

		void SpriteGraphicsMemoryManager::Clear()
		{
			m_paletteRefTracker.SetAllTo(0);
		}
	}
}
