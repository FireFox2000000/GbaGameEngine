#include "GBASpriteManager.h"
#include "GBASprite.h"
#include "GBASpriteAtlus.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVram.h"

namespace GBA
{
	namespace Gfx
	{
		SpriteManager::SpriteManager()
			: m_paletteRefTracker(0)
		{
		}

		SpriteManager::~SpriteManager()
		{
		}

		void SpriteManager::Load(Sprite& out_sprite)
		{
			using namespace GBA;

			if (out_sprite.IsLoaded())
				return;

			// Load palette
			SpriteAtlus* atlus = out_sprite.EditAtlus();
			tPaletteIndex paletteId = 0;

			if (!out_sprite.m_atlus->IsPaletteLoaded())
			{
				while (paletteId < m_paletteRefTracker.Count() && m_paletteRefTracker[paletteId] > 0)
					++paletteId;

				if (paletteId >= m_paletteRefTracker.Count())
					return;		// Out of palette space!

				ColourPalette16 palette(0);

				for (u32 i = 0; i < atlus->m_paletteLength; ++i)
				{
					palette[i] = atlus->m_palette[i];
				}
				PaletteBank::LoadSpritePalette(paletteId, palette);
				atlus->m_paletteIndex = paletteId;
			}
			else
			{
				paletteId = out_sprite.GetPaletteIndex();
			}

			++m_paletteRefTracker[paletteId];

			u32 compressionFlags = out_sprite.m_atlus->GetSpriteDataCompressionFlags();
			tTileId tileIndex = GBA::Vram::GetInstance().AllocSpriteMem(out_sprite.m_pixelMapData, out_sprite.m_pixelMapDataLength, compressionFlags);

			if (tileIndex != INVALID_TILE_ID)
			{
				// Set sprite attributes
				out_sprite.m_renderData.SetTileIndex(tileIndex);
			}
			else
			{
				// Undo the palette we just loaded
				--m_paletteRefTracker[paletteId];
			}
		}

		void SpriteManager::Unload(Sprite * sprite)
		{
			if (!sprite->IsLoaded())
				return;

			// Remove tile references
			tTileId index = sprite->m_renderData.GetTileIndex();
			GBA::Vram::GetInstance().FreeSpriteMem(index);
			sprite->m_renderData.SetTileIndex(INVALID_TILE_ID);

			// Decrease palette references
			--m_paletteRefTracker[sprite->GetPaletteIndex()];
			if (m_paletteRefTracker[sprite->GetPaletteIndex()] <= 0)
			{
				// Free it so it can be reused
				SpriteAtlus* atlus = sprite->m_atlus;
				atlus->m_paletteIndex = INVALID_PALETTE_INDEX;
			}
		}

		void SpriteManager::Clear()
		{
			m_paletteRefTracker.SetAllTo(0);
		}
	}
}
