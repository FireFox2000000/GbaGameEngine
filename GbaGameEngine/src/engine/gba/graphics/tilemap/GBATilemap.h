#pragma once
#include "engine/base/core/stl/Span.h"
#include "engine/math/Vector2.h"
#include "engine/gba/graphics/vram/GBAVramAllocator.h"
#include "engine/gba/registers/display/GBABackgroundAllocator.h"

namespace GBA 
{
	class Graphics;
}

namespace GBATEK
{
	struct BackgroundTilemapEntry;
}

namespace GBA
{
	namespace Gfx
	{
		class TilemapSet;

		class Tilemap
		{
		public:
			struct FileDataMap
			{
				Span<const GBATEK::BackgroundTilemapEntry> m_tileMapEntries;
				bool m_isDynamicallyRendered = false;		// i.e. not GBA nested
				Vector2<u8> m_sizeInTiles;
			};

		private:
			friend class TilemapSet;
			friend class TilemapManager;
			friend class GBA::Graphics;

			struct RenderData
			{
				// Runtime assigned render data when loaded
				GBA::VramAllocator::tScreenBaseBlockIndex m_mapSbbIndex = GBA::VramAllocator::INVALID_SBB_ID;
				int m_backgroundSlotId = GBA::BackgroundAllocator::INVALID_BACKGROUND;

				/*
				* Used for rendering optimisations to track what's currently already loaded into memory.
				* Background positions are write-only, so we need to track this manually.
				*/
				Vector2i lastRenderPos;
				bool lastRenderPosValid = false;
			};

			TilemapSet* m_tilemapSet = nullptr;
			TilemapSet* EditTilemapSet();

			FileDataMap m_file;
			RenderData m_renderData;

		public:
			Tilemap();

			bool IsLoaded() const;
			bool IsDynamicallyRendered() const;
			const Vector2<u8>& GetSizeInTiles() const;
			const TilemapSet* GetTilemapSet() const;
			Span<const GBATEK::BackgroundTilemapEntry> GetTileMapEntries() const;
			GBA::VramAllocator::tScreenBaseBlockIndex GetMapScreenBaseBlockIndex() const;
			int GetAssignedBackgroundSlot() const;
			const FileDataMap& GetFile() const { return m_file; }
		};
	}
}
