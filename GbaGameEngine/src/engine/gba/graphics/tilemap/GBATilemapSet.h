#pragma once
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/tilemap/GBATilemap.h"
#include "engine/base/colour/Palette.h"
#include "engine/gba/graphics/vram/GBAVram.h"

namespace GBA
{
	namespace Gfx
	{
		class TilemapSet
		{
			friend class TilemapManager;

		public:
			const static GBA::TileBlockGroups INVALID_TILESET_CBB = GBA::TileBlockGroups::BlockGroupCount;

			struct FileDataMap
			{
				const u16* m_palette = NULL;
				u8 m_paletteLength = 0;

				const u32* m_tileset = NULL;
				u32 m_tilesetLength = 0;

				u32 m_tileSetDataCompressionFlags = 0;
			};

			struct RenderData
			{
				// Runtime assigned render data when loaded
				tPaletteIndex m_paletteIndex = INVALID_PALETTE_INDEX;
				GBA::TileBlockGroups m_tileSetCharacterBaseBlock = INVALID_TILESET_CBB;
			};

			List<Tilemap> m_maps;
			FileDataMap m_file;
			RenderData m_renderData;

		public:
			TilemapSet();
			TilemapSet(
				const u8 paletteLength,
				const u16* palette,
				const u32 tilesetLength,
				const u32* tileset,
				const u32 tileSetDataCompressionFlags,
				const u8 mapCount,
				const u8 mapIsDynamicMask,
				const u8* mapTileWidths,
				const u8* mapTileHeights,
				const u16* mapData
			);

			TilemapSet(const TilemapSet & that);

			bool IsPaletteLoaded() const;
			inline tPaletteIndex GetPaletteIndex() const { return m_renderData.m_paletteIndex; }

			bool IsTileSetLoaded() const;
			inline GBA::TileBlockGroups GetTileSetCharacterBaseBlock() const { return m_renderData.m_tileSetCharacterBaseBlock; }

			bool IsLoaded() const;

			Tilemap* GetTilemap(int index) {
				if (index >= 0 && index < (int)m_maps.Count())
					return &m_maps[index];
				else
					return nullptr;
			}
		};
	}
}
