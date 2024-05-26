#pragma once
#include "engine/math/Vector2.h" 
#include "engine/base/colour/Palette.h"
#include "engine/gba/graphics/tiles/GBATile.h"
#include "GBASDK/ObjectAttributeMemory.h"

namespace GBA
{
	union UPixelData;
}

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
			friend class SpriteAtlus;
			friend class OAMManager;

			ObjectShape m_objectShape;
			ObjectSize m_objectSize;

			Vector2<u8> m_tileSize;
			RenderData m_renderData;

			SpriteAtlus* m_atlus;
			const GBA::UPixelData* m_pixelMapData;
			u32 m_pixelMapDataLength;

			SpriteAtlus* EditAtlus();
		public:
			Sprite();
			Sprite(ObjectShape shape, ObjectSize sizeMode);
			~Sprite();

			ObjectShape GetShape() const;
			ObjectSize GetSizeMode() const;
			tTileId GetTileIndex() const;
			tPaletteIndex GetPaletteIndex() const;
			const SpriteAtlus* GetAtlus() const;
			bool IsLoaded() const;

			Vector2<int> GetSize() const;
			Vector2<int> GetSizeInPixels() const;
		};
	}
}
