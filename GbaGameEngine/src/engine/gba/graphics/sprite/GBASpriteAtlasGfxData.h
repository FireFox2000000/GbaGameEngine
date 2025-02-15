#pragma once
#include "engine/base/core/stl/List.h"
#include "engine/base/core/stl/Span.h"
#include "engine/base/colour/Palette.h"

template<typename T>
class IPool;
class MemoryMappedFileStream;

class Sprite;
struct SpriteNode;

namespace GBA
{
	namespace Gfx
	{
		class SpriteAtlasGfxData
		{
			friend class SpriteGraphicsMemoryManager;	// palette, palette index

			u32 m_spriteDataCompressionFlags = 0;
			Span<const GBATEK::ColourRGB16> m_palette;

			tPaletteIndex m_paletteIndex = INVALID_PALETTE_INDEX;

		public:
			SpriteAtlasGfxData() = default;
			SpriteAtlasGfxData(u32 compressionFlags, Span<const GBATEK::ColourRGB16> paletteData)
				: m_spriteDataCompressionFlags(compressionFlags)
				, m_palette(paletteData)
			{}

			tPaletteIndex GetPaletteIndex() const { return m_paletteIndex; }
			bool IsPaletteLoaded() const;
		};
	}
}
