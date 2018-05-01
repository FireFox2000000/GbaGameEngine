#ifndef PRAGMA_ONCE_ENGINE_ASSETS_GBASPRITE_H
#define PRAGMA_ONCE_ENGINE_ASSETS_GBASPRITE_H

#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/tiles/GBATileBank.h"

namespace GBA
{
	class Sprite
	{
		friend class SpriteLoader;

		GBA::Attributes::Shape m_shape;
		GBA::Attributes::SizeMode m_sizeMode;
		GBA::tTileId m_tileIndex;
		GBA::tPaletteBlockId m_paletteId;
		bool m_isLoaded;

	public:
		Sprite()
			: m_shape(GBA::Attributes::Square)
			, m_sizeMode(GBA::Attributes::Form0)
			, m_tileIndex(0)
			, m_paletteId(0)
			, m_isLoaded(false)
		{}

		inline GBA::Attributes::Shape GetShape() { return m_shape; }
		inline GBA::Attributes::SizeMode GetSizeMode() { return m_sizeMode; }
		inline GBA::tTileId GetTileIndex() { return m_tileIndex; }
		inline GBA::tPaletteBlockId GetPaletteIndex() { return m_paletteId; }
		inline bool GetIsLoaded() { return m_isLoaded; }
	};
}

#endif
