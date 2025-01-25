#include "GBASprite.h"
#include "GBASpriteAtlas.h"
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

		SpriteAtlas* Sprite::EditAtlas()
		{
			return m_atlas;
		}

		Sprite::Sprite()
			: m_objectShape(GBATEK::ObjectShape::Square)
			, m_objectSize(GBATEK::ObjectSize::Smallest)
			, m_tileSize(Vector2<u8>())
			, m_atlas(nullptr)
		{
		}

		Sprite::Sprite(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode)
			: m_objectShape(shape)
			, m_objectSize(sizeMode)
			, m_tileSize(AttributeFunctions::GetTileSize(shape, sizeMode))
			, m_atlas(nullptr)
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
			return m_atlas ? m_atlas->GetPaletteIndex() : INVALID_PALETTE_INDEX;
		}

		const SpriteAtlas* Sprite::GetAtlas() const
		{
			return m_atlas;
		}

		bool Sprite::IsLoaded() const
		{
			return GetTileIndex() != INVALID_TILE_ID && m_atlas->IsPaletteLoaded();
		}

		Vector2<int> Sprite::GetSize() const
		{
			return Vector2<int>(m_tileSize);
		}

		Vector2<int> Sprite::GetSizeInPixels() const
		{
			return Vector2<int>(AttributeFunctions::GetPixelSize(m_tileSize));
		}
	}
}
