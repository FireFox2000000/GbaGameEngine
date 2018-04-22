#ifndef PRAGMA_ONCE_ENGINE_ASSETS_GBASPRITE_H
#define PRAGMA_ONCE_ENGINE_ASSETS_GBASPRITE_H

#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/tiles/GBATileBank.h"

namespace GBA
{
	struct Sprite
	{
		GBA::Attributes::Shape shape;
		GBA::Attributes::SizeMode sizeMode;
		GBA::tTileId tileIndex;
		GBA::tPaletteBlockId paletteId;
		bool isLoaded;

		Sprite()
			: shape(GBA::Attributes::Square)
			, sizeMode(GBA::Attributes::Form0)
			, tileIndex(0)
			, paletteId(0)
			, isLoaded(false)
		{}

		// Temp method, probably replace when a proper sprite loader is created
		void Init(GBA::Attributes::Shape shape, GBA::Attributes::SizeMode sizeMode, GBA::tPaletteBlockId palette, GBA::tTileId tileIndex)
		{
			this->shape = shape;
			this->sizeMode = sizeMode;
			this->paletteId = palette;
			this->tileIndex = tileIndex;
			isLoaded = true;
		}
	};
}

#endif
