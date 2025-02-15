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
		class OAMManager;

		class SpriteGfxData
		{
			friend class SpriteGraphicsMemoryManager;	// pixelmap and tile index
			friend class OAMManager;	// addedtodraw, shape/size, tileindex

			u16 m_tileIndex;
			bool m_addedToDrawList;

			Span<const GBATEK::UPixelData> m_pixelMapData;

			GBATEK::ObjectShape m_objectShape = GBATEK::ObjectShape::Square;
			GBATEK::ObjectSize m_objectSize = GBATEK::ObjectSize::Smallest;

		public:
			SpriteGfxData()
			{
				m_tileIndex = INVALID_TILE_ID;
				m_addedToDrawList = false;
			}

			SpriteGfxData(Span<const GBATEK::UPixelData> pixelMap, GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode)
				: m_pixelMapData(pixelMap)
				, m_objectShape(shape)
				, m_objectSize(sizeMode)
			{
				m_tileIndex = INVALID_TILE_ID;
				m_addedToDrawList = false;
			}

			u16 GetTileIndex() const { return m_tileIndex; }
		};
	}
}
