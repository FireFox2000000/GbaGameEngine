#pragma once
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/tilemap/GBATilemap.h"
#include "engine/base/colour/Palette.h"

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
				u8 m_paletteBankIndex = 0;

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
				const u8 paletteBankIndex,
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
			tPaletteIndex GetPaletteIndex() const;

			bool IsTileSetLoaded() const;
			GBA::TileBlockGroups GetTileSetCharacterBaseBlock() const;

			bool IsLoaded() const;

			Tilemap* GetTilemap(int index);
		};
	}
}
