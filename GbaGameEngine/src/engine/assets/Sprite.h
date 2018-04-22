#ifndef PRAGMA_ONCE_ENGINE_ASSETS_SPRITE_H
#define PRAGMA_ONCE_ENGINE_ASSETS_SPRITE_H

#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/tiles/GBATileBank.h"


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

	void Init(GBA::Attributes::Shape shape, GBA::Attributes::SizeMode sizeMode, GBA::tPaletteBlockId palette, GBA::tTileId tileIndex)
	{
		this->shape = shape;
		this->sizeMode = sizeMode;
		this->paletteId = palette;
		this->tileIndex = tileIndex;
		isLoaded = true;
	}
};

#endif
