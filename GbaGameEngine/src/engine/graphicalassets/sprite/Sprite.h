#pragma once
#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/graphicalassets/tile/Tile.h"

class SpriteAtlus;

namespace GBA
{
	class OAMManager;
}

class Sprite
{
public:
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
	friend class GBA::OAMManager;

	GBA::Attributes::Shape m_shape;
	GBA::Attributes::SizeMode m_sizeMode;
	RenderData m_renderData;

	SpriteAtlus* m_atlus;
	const u32* m_pixelMapData;
	u32 m_pixelMapDataLength;

	SpriteAtlus* EditAtlus() { return m_atlus; }
public:
	Sprite();

	inline GBA::Attributes::Shape GetShape() const { return m_shape; }
	inline GBA::Attributes::SizeMode GetSizeMode() const { return m_sizeMode; }
	inline tTileId GetTileIndex() const { return m_renderData.GetTileIndex(); }
	tPaletteIndex GetPaletteIndex() const;
	inline const SpriteAtlus* GetAtlus() const { return m_atlus; }
	bool IsLoaded() const;
};
