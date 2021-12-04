#pragma once
#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/tiles/GBATile.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"

class SpriteAssetManagerHelper;

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

				u16 m_dataMask = 0;

			public:
				tTileId GetTileIndex() const;
				void SetTileIndex(tTileId index);

				inline bool IsAddedToDrawList() const { return m_dataMask & BIT(DRAW_LIST_FLAG_INDEX); }
				void SetAddedToDrawList(bool val);
			};

		private:
			friend class SpriteGraphicsMemoryManager;
			friend class ::SpriteAssetManagerHelper;
			friend class SpriteAtlus;
			friend class OAMManager;

			u8 m_attributes;		// First 2 bits stores Attributes::Shape, next 2 bits stores Attributes::SizeMode
			Vector2<u8> m_tileSize;

			RenderData m_renderData;

			SpriteAtlus* m_atlus;
			const u32* m_pixelMapData;
			u32 m_pixelMapDataLength;

			SpriteAtlus* EditAtlus() { return m_atlus; }
		public:
			Sprite();
			Sprite(Attributes::Shape shape, Attributes::SizeMode sizeMode);
			~Sprite() { DEBUG_ASSERTMSG(!IsLoaded(), "Sprite was destroyed while it was still loaded in video memory!"); }

			inline Attributes::Shape GetShape() const { return (Attributes::Shape)(m_attributes & ~(0xFF << 2)); }
			inline Attributes::SizeMode GetSizeMode() const { return (Attributes::SizeMode)(m_attributes >> 2); }
			inline tTileId GetTileIndex() const { return m_renderData.GetTileIndex(); }
			tPaletteIndex GetPaletteIndex() const;
			inline const SpriteAtlus* GetAtlus() const { return m_atlus; }
			bool IsLoaded() const;

			inline Vector2<int> GetSize() const { return m_tileSize; }
			inline Vector2<int> GetSizeInPixels() const { return AttributeFunctions::GetPixelSize(m_tileSize); }
		};
	}
}
