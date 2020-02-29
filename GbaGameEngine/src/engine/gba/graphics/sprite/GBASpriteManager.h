#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/colour/Palette.h"
#include "engine/gba/graphics/tiles/GBATile.h"

namespace GBA
{
	namespace Gfx
	{
		class Sprite;

		class SpriteManager
		{
			Array<u32, sizeof(ColourPalette16) / sizeof(rgb16)> m_paletteRefTracker;

		public:
			SpriteManager();
			~SpriteManager();

			void Load(Sprite& out_sprite);
			void Unload(Sprite* sprite);
			void Clear();
		};
	}
}
