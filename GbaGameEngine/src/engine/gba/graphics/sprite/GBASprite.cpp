#include "GBASprite.h"
#include "GBASpriteAtlus.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"

const u8 TILE_INDEX_MAX_BITS = 10;
const u8 DRAW_LIST_FLAG_INDEX = 11;

namespace GBA
{
	namespace Gfx
	{
		tTileId Sprite::RenderData::GetTileIndex() const
		{
			return m_tileIndex;
		}

		void Sprite::RenderData::SetTileIndex(tTileId index)
		{
			m_tileIndex = index;
		}

		bool Sprite::RenderData::IsAddedToDrawList() const
		{
			return m_addedToDrawList;
		}

		void Sprite::RenderData::SetAddedToDrawList(bool val)
		{
			m_addedToDrawList = val;
		}

		///////////////////////////////////////////////////////////

		SpriteAtlus* Sprite::EditAtlus()
		{
			return m_atlus;
		}

		Sprite::Sprite()
			: m_objectShape(GBATEK::ObjectShape::Square)
			, m_objectSize(GBATEK::ObjectSize::Smallest)
			, m_tileSize(GBAAttrFnVector2())
			, m_atlus(nullptr)
			, m_pixelMapData(nullptr)
			, m_pixelMapDataLength(0)
		{
		}

		Sprite::Sprite(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode)
			: m_objectShape(shape)
			, m_objectSize(sizeMode)
			, m_tileSize(AttributeFunctions::GetTileSize(shape, sizeMode))
			, m_atlus(nullptr)
			, m_pixelMapData(nullptr)
			, m_pixelMapDataLength(0)
		{
			m_renderData.SetTileIndex(INVALID_TILE_ID);
		}

		Sprite::~Sprite()
		{
			DEBUG_ASSERTMSG(!IsLoaded(), "Sprite was destroyed while it was still loaded in video memory!");
		}

		GBATEK::ObjectShape Sprite::GetShape() const
		{
			return m_objectShape;
		}

		GBATEK::ObjectSize Sprite::GetSizeMode() const
		{
			return m_objectSize;
		}

		tTileId Sprite::GetTileIndex() const
		{
			return m_renderData.GetTileIndex();
		}

		tPaletteIndex Sprite::GetPaletteIndex() const
		{
			return m_atlus ? m_atlus->GetPaletteIndex() : INVALID_PALETTE_INDEX;
		}

		const SpriteAtlus* Sprite::GetAtlus() const
		{
			return m_atlus;
		}

		bool Sprite::IsLoaded() const
		{
			return GetTileIndex() != INVALID_TILE_ID && m_atlus->IsPaletteLoaded();
		}

		Vector2<int> Sprite::GetSize() const
		{
			return m_tileSize;
		}

		Vector2<int> Sprite::GetSizeInPixels() const
		{
			return AttributeFunctions::GetPixelSize(m_tileSize);
		}
	}
}
