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

		// Use this to load a sprite's palette and load tile information into vram. 
		// Keeps a reference per sprite atlas of what's loaded, so the atlas/palette will automatically unload once all sprites that reference the same atlas are unloaded.
		class SpriteGraphicsMemoryManager
		{
			Array<u32, sizeof(ColourPalette16) / sizeof(GBATEK::ColourRGB16)> m_paletteRefTracker;

		public:
			SpriteGraphicsMemoryManager();
			~SpriteGraphicsMemoryManager();

			void Load(Sprite& out_sprite);
			void Unload(Sprite* sprite);
			void Clear();
		};
	}
}
