#pragma once
#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/tiles/GBATile.h"

namespace GBA
{

	namespace Gfx
	{
		class SpriteAtlus;
		class OAMManager;

		// Holds onto the actual sprite pixel data and defines the properties of the sprite.
		class Sprite
		{
		public:
			// Tracks live render information, such as vram memory location etc.
			// Only used by draw functions. 
			class RenderData
			{
				static const u8 TILE_INDEX_MAX_BITS = 10;
				static const u8 DRAW_LIST_FLAG_INDEX = 11;

				u16 m_dataMask;

			public:
				tTileId GetTileIndex() const;
				void SetTileIndex(tTileId index);

				inline bool IsAddedToDrawList() const { return m_dataMask & BIT(DRAW_LIST_FLAG_INDEX); }
				void SetAddedToDrawList(bool val);
			};

		private:
			friend class SpriteManager;
			friend class SpriteAtlus;
			friend class OAMManager;

			Attributes::Shape m_shape;
			Attributes::SizeMode m_sizeMode;
			RenderData m_renderData;

			SpriteAtlus* m_atlus;
			const u32* m_pixelMapData;
			u32 m_pixelMapDataLength;

			SpriteAtlus* EditAtlus() { return m_atlus; }
		public:
			Sprite();

			inline Attributes::Shape GetShape() const { return m_shape; }
			inline Attributes::SizeMode GetSizeMode() const { return m_sizeMode; }
			inline tTileId GetTileIndex() const { return m_renderData.GetTileIndex(); }
			tPaletteIndex GetPaletteIndex() const;
			inline const SpriteAtlus* GetAtlus() const { return m_atlus; }
			bool IsLoaded() const;

			Vector2<int> GetSize() const;
			Vector2<int> GetSizeInPixels() const;
		};
	}
}
