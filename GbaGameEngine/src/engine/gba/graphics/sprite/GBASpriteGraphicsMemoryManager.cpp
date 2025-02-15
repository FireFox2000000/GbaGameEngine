#include "GBASpriteGraphicsMemoryManager.h"
#include "engine/graphics/sprite/Sprite.h"
#include "engine/graphics/sprite/SpriteAtlas.h"
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

			if (!out_sprite.GetAtlas()->GetGfxData().IsPaletteLoaded())
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

				for (u32 i = 0; i < atlas->GetGfxData().m_palette.Count(); ++i)
				{
					palette[i] = atlas->GetGfxData().m_palette[i];
				}
				PaletteBank::LoadSpritePalette(paletteId, palette);
				atlas->EditGfxData().m_paletteIndex = paletteId;
			}
			else
			{
				paletteId = out_sprite.GetAtlas()->GetGfxData().GetPaletteIndex();
			}

			++m_paletteRefTracker[paletteId];

			u32 compressionFlags = out_sprite.GetAtlas()->GetGfxData().m_spriteDataCompressionFlags;
			tTileId tileIndex = GBA::VramAllocator::GetInstance().AllocSpriteMem(out_sprite.GetGfxData().m_pixelMapData.Data(), out_sprite.GetGfxData().m_pixelMapData.Count(), compressionFlags);

			if (tileIndex != INVALID_TILE_ID)
			{
				// Set sprite attributes
				out_sprite.EditGfxData().m_tileIndex = tileIndex;
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
			tTileId index = sprite->GetGfxData().m_tileIndex;
			GBA::VramAllocator::GetInstance().FreeSpriteMem(index);
			sprite->EditGfxData().m_tileIndex = INVALID_TILE_ID;

			// Decrease palette references
			auto paletteIndex = sprite->GetAtlas()->GetGfxData().m_paletteIndex;
			--m_paletteRefTracker[paletteIndex];
			if (m_paletteRefTracker[paletteIndex] <= 0)
			{
				// Free it so it can be reused
				SpriteAtlas* atlas = sprite->EditAtlas();
				atlas->EditGfxData().m_paletteIndex = INVALID_PALETTE_INDEX;
			}
		}

		void SpriteGraphicsMemoryManager::Clear()
		{
			m_paletteRefTracker.SetAllTo(0);
		}
	}
}
