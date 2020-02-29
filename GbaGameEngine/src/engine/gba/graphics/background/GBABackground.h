#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"
#include "engine/gba/graphics/vram/GBAVram.h"
#include "engine/gba/graphics/GBADrawPriority.h"

namespace GBA
{
	namespace Gfx
	{
		namespace Background
		{
			using tCharacterBaseBlock = TileBlockGroups;

			struct ControlRegister
			{
			private:
				u16 m_registerValues;

				void SetControlRegister(int value, u16 attributeMask) { m_registerValues = (m_registerValues & ~attributeMask) | (value & attributeMask); }
			public:

				enum ColourMode
				{
					FourBitsPerPixel,
					EightBitsPerPixel
				};

				enum Size
				{
					REG_32x32 = 0,
					REG_64x32 = 1,
					REG_32x64 = 2,
					REG_64x64 = 3,
					REGSize_Count = 4,

					AFF_16x16 = 0,
					AFF_32x32 = 1,
					AFF_64x64 = 2,
					AFF_128x128 = 3,
					AFFSize_Count = 4,
				};

				void SetPriority(DrawPriority priority);
				void SetCharacterBaseBlock(tCharacterBaseBlock blockId);
				void SetMosaic(bool enabled);
				void SetColourMode(ColourMode colourMode);
				void SetScreenBaseBlock(tScreenBaseBlockIndex blockId);
				void SetAffineWrapping(bool enabled);
				void SetSize(Size size);
			};

			// Background position is write only
			class Position
			{
				s16 m_xOffset, m_yOffset;

			public:
				void SetX(s16 val) { m_xOffset = val; }
				void SetY(s16 val) { m_yOffset = val; }
			};

			ControlRegister::ColourMode GetColourModeFromCompression(u32 compressionFlags);
			ControlRegister::Size GetRegSizeFromTileSize(u8 width, u8 height);
		}
	}
}
