#ifndef PRAGMA_ONCE_ENGINE_ASSETS_SPRITE_SPRITELOADER_H
#define PRAGMA_ONCE_ENGINE_ASSETS_SPRITE_SPRITELOADER_H

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"

class Sprite;

class SpriteLoader
{
	struct SpriteData
	{
		GBA::Attributes::Shape shape;
		GBA::Attributes::SizeMode size;
		u8 paletteLength;
		u32 pixelMapLength;
		const u16* palette;
		const u16* pixelMapData;

		int paletteIndex;
	};

	List<SpriteData> m_spriteData;
	List<Sprite> m_spriteList;
	Array<u32, sizeof(GBA::ColourPalette16) / sizeof(rgb16)> m_paletteRefTracker;

	SpriteLoader();

public:
	~SpriteLoader();

	static SpriteLoader* GetCurrent()
	{
		static SpriteLoader instance;
		return &instance;
	}

	void Load(Sprite& out_sprite);
	void Dispose(Sprite* sprite);
	void Clear();
};

#endif
