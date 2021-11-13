#pragma once
#include "engine/gba/graphics/background/GBABackground.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"

namespace GBA {
	class Graphics;
}

namespace GBA
{
	namespace Gfx
	{
		class TilemapSet;

		class Tilemap
		{
			friend class TilemapSet;
			friend class TilemapManager;
			friend class GBA::Graphics;

			struct FileDataMap
			{
				const u16* m_tileMapData = NULL;
				u16 m_tileMapDataLength = 0;
				bool m_isDynamicallyRendered = false;		// i.e. not GBA nested
				Vector2<u8> m_sizeInTiles;
			};

			struct RenderData
			{
				// Runtime assigned render data when loaded
				GBA::tScreenBaseBlockIndex m_mapSbbIndex = GBA::INVALID_SBB_ID;
				GBA::BackgroundControl::Backgrounds m_backgroundSlotId = GBA::BackgroundControl::Count;

				/*
				* Used for rendering optimisations to track what's currently already loaded into memory.
				* Background positions are write-only, so we need to track this manually.
				*/
				Vector2<int> lastRenderPos;
				bool lastRenderPosValid = false;
			};

			TilemapSet* m_tilemapSet = NULL;
			TilemapSet* EditTilemapSet() { return m_tilemapSet; }

			FileDataMap m_file;
			RenderData m_renderData;

			GBA::Gfx::Background::ControlRegister::Size GetSize() const;

		public:
			Tilemap();

			bool IsLoaded() const;
			bool IsDynamicallyRendered() const { return m_file.m_isDynamicallyRendered; }
			inline const Vector2<u8>& GetSizeInTiles() const { return m_file.m_sizeInTiles; }
			inline const TilemapSet* GetTilemapSet() const { return m_tilemapSet; }
			inline u16 GetTileMapLength() const { return m_file.m_tileMapDataLength; }
			inline const u16* GetTileMapData() const { return m_file.m_tileMapData; }
			inline GBA::tScreenBaseBlockIndex GetMapScreenBaseBlockIndex() const { return m_renderData.m_mapSbbIndex; }
			inline GBA::BackgroundControl::Backgrounds GetAssignedBackgroundSlot() const { return m_renderData.m_backgroundSlotId; }
		};
	}
}
