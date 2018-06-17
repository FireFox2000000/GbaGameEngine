#ifndef PRAGMA_ONCE_ENGINE_GRAPHICALASSETS_SPRITE_SPRITE_H
#define PRAGMA_ONCE_ENGINE_GRAPHICALASSETS_SPRITE_SPRITE_H

#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/graphicalassets/tile/Tile.h"

class SpriteAtlus;
class Sprite
{
	friend class SpriteManager;
	friend class SpriteLibrary;

	GBA::Attributes::Shape m_shape;
	GBA::Attributes::SizeMode m_sizeMode;
	tTileId m_tileIndex;

	SpriteAtlus* m_atlus;
	const u16* m_pixelMapData;
	u32 m_pixelMapDataLength;

	SpriteAtlus* EditAtlus() { return m_atlus; }
public:
	Sprite();

	inline GBA::Attributes::Shape GetShape() const { return m_shape; }
	inline GBA::Attributes::SizeMode GetSizeMode() const { return m_sizeMode; }
	inline tTileId GetTileIndex() const { return m_tileIndex; }
	tPaletteIndex GetPaletteIndex() const;
	inline const SpriteAtlus* GetAtlus() const { return m_atlus; }
	bool IsLoaded() const;
};

#endif
