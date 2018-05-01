#ifndef PRAGMA_ONCE_ENGINE_ASSETS_GBASPRITELOADER_H
#define PRAGMA_ONCE_ENGINE_ASSETS_GBASPRITELOADER_H

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"

// Remember to #include the sprite data in GBASpriteLoader.cpp
#define SPRITE_ATLUS_LIST \
	SPRITE_ATLUS_ENTRY(Shantae)

namespace GBA
{
	class Sprite;

	namespace SpriteAtlus
	{
#define SPRITE_ATLUS_ENTRY(Namespace) Namespace,
		enum Enum
		{
			SPRITE_ATLUS_LIST
			Count
		};
#undef SPRITE_ATLUS_ENTRY
	}

	class SpriteLoader
	{
		struct SpriteData
		{
			Attributes::Shape shape;
			Attributes::SizeMode size;
			u8 paletteLength;
			u32 pixelMapLength;
			const u16* palette;
			const u16* pixelMapData;

			int paletteIndex;
		};

		List<SpriteData> m_spriteData;
		List<Sprite> m_spriteList;
		Array<u32, sizeof(ColourPalette16) / sizeof(rgb16)> m_paletteRefTracker;

		SpriteLoader();

	public:
		~SpriteLoader();

		static SpriteLoader* GetCurrent()
		{
			static SpriteLoader instance;
			return &instance;
		}

		Sprite* Load(SpriteAtlus::Enum spriteAtlus);
		void Dispose(Sprite* sprite);
		void Clear();
	};
}

#endif
