#pragma once
#include "engine/math/Vector2.h" 
#include "engine/base/core/stl/Span.h"
#include "engine/base/colour/Palette.h"
#include "engine/gba/graphics/tiles/GBATile.h"
#include "gbatek/ObjectAttributeMemory.h"

namespace GBATEK
{
	union UPixelData;
}

namespace GBA
{
	namespace Gfx
	{
		class SpriteAtlas;
		class OAMManager;

		// Holds onto the actual sprite pixel data and defines the properties of the sprite.
		class Sprite
		{
		public:
			// Tracks live render information, such as vram memory location etc.
			// Only used by draw functions. 
			class RenderData
			{
				u16 m_tileIndex : 10
					, m_addedToDrawList : 1;

			public:
				tTileId GetTileIndex() const;
				void SetTileIndex(tTileId index);

				bool IsAddedToDrawList() const;
				void SetAddedToDrawList(bool val);
			};

		private:
			friend class SpriteGraphicsMemoryManager;
			friend class SpriteAtlas;
			friend class OAMManager;

			GBATEK::ObjectShape m_objectShape;
			GBATEK::ObjectSize m_objectSize;

			Vector2<u8> m_tileSize;
			RenderData m_renderData;

			SpriteAtlas* m_atlas = nullptr;
			Span<const GBATEK::UPixelData> m_pixelMapData;

			SpriteAtlas* EditAtlas();
		public:
			Sprite();
			Sprite(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode);
			~Sprite();

			GBATEK::ObjectShape GetShape() const;
			GBATEK::ObjectSize GetSizeMode() const;
			tTileId GetTileIndex() const;
			tPaletteIndex GetPaletteIndex() const;
			const SpriteAtlas* GetAtlas() const;
			bool IsLoaded() const;

			Vector2i GetSize() const;
			Vector2i GetSizeInPixels() const;
		};
	}
}
