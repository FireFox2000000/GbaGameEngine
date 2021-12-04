#include "GBASprite.h"
#include "GBASpriteAtlus.h"
#include "engine/base/colour/Palette.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"

namespace GBA
{
	namespace Gfx
	{
		tTileId Sprite::RenderData::GetTileIndex() const
		{
			return m_dataMask & BITS_U32(TILE_INDEX_MAX_BITS);
		}

		void Sprite::RenderData::SetTileIndex(tTileId index)
		{
			m_dataMask = (m_dataMask & ~BITS_U32(TILE_INDEX_MAX_BITS)) | index;
		}

		void Sprite::RenderData::SetAddedToDrawList(bool val)
		{
			if (val)
			{
				m_dataMask |= BIT(DRAW_LIST_FLAG_INDEX);
			}
			else
			{
				m_dataMask &= ~BIT(DRAW_LIST_FLAG_INDEX);
			}
		}

		///////////////////////////////////////////////////////////

		Sprite::Sprite()
			: m_attributes(0)
			, m_tileSize(GBAAttrFnVector2())
			, m_atlus(NULL)
			, m_pixelMapData(NULL)
			, m_pixelMapDataLength(0)
		{
		}

		Sprite::Sprite(Attributes::Shape shape, Attributes::SizeMode sizeMode)
			: m_attributes(shape | (sizeMode << 2))
			, m_tileSize(AttributeFunctions::GetTileSize(shape, sizeMode))
			, m_atlus(NULL)
			, m_pixelMapData(NULL)
			, m_pixelMapDataLength(0)
		{
			m_renderData.SetTileIndex(INVALID_TILE_ID);
		}

		tPaletteIndex Sprite::GetPaletteIndex() const
		{
			return m_atlus ? m_atlus->GetPaletteIndex() : INVALID_PALETTE_INDEX;
		}

		bool Sprite::IsLoaded() const
		{
			return GetTileIndex() != INVALID_TILE_ID && m_atlus->IsPaletteLoaded();
		}
	}
}
