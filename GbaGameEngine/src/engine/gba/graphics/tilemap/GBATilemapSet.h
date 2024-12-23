#pragma once
#include "engine/base/core/stl/List.h"
#include "engine/base/core/stl/Span.h"
#include "engine/gba/graphics/tilemap/GBATilemap.h"
#include "engine/base/colour/Palette.h"

namespace GBATEK
{
	union UPixelData;
	struct ColourRGB16;
	enum class BackgroundColourMode : unsigned char;
}

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
				Span<const GBATEK::ColourRGB16> m_palette;
				Span<const GBATEK::UPixelData> m_tileset;
				u8 m_paletteBankIndex = 0;
				GBATEK::BackgroundColourMode m_backgroundColourMode;
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
				const Span<const GBATEK::ColourRGB16> palette,
				const Span<const GBATEK::UPixelData> tileset,
				const u32 tileSetDataCompressionFlags,
				const u8 mapCount,
				const u8 mapIsDynamicMask,
				const Span<const u8> mapTileWidths,
				const Span<const u8> mapTileHeights,
				const Span<const GBATEK::BackgroundTilemapEntry> mapData
			);

			TilemapSet(const TilemapSet & that);

			bool IsPaletteLoaded() const;
			tPaletteIndex GetPaletteIndex() const;

			bool IsTileSetLoaded() const;
			GBA::TileBlockGroups GetTileSetCharacterBaseBlock() const;

			bool IsLoaded() const;

			Tilemap* GetTilemap(int index);
			const Tilemap* GetTilemap(int index) const;
		};
	}
}
