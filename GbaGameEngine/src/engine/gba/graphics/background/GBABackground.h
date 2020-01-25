#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"
#include "engine/gba/graphics/vram/GBAVram.h"

namespace GBA
{
	class Background
	{
	public:
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

			enum Priority
			{
				Layer0,
				Layer1,
				Layer2,
				Layer3
			};

			void SetPriority(Priority priority);
			void SetCharacterBaseBlock(tCharacterBaseBlock blockId);
			void SetMosaic(bool enabled);
			void SetColourMode(ColourMode colourMode);
			void SetScreenBaseBlock(tScreenBaseBlockIndex blockId);
			void SetAffineWrapping(bool enabled);
			void SetSize(Size size);
		};

	private:
		// Background position is read only, better to hide this
		struct Position
		{
			s16 xOffset, yOffset;
		};

	private:
		u8 m_index = 0;

	public:
		Background() {}; // Purely to make this class std::initializer_list compliant
		Background(u8 index) : m_index(index) {}

		ControlRegister& EditControlRegister();

		static ControlRegister::ColourMode GetColourModeFromCompression(u32 compressionFlags);

		template<class IntType, u8 BITS>
		inline void SetPosition(const Vector2<FixedPoint<IntType, BITS> >& position) // Position of the screen on the map
		{
			Position* bgPos = reinterpret_cast<Position*>(&EditControlRegister() + 4 + m_index);
			bgPos->xOffset = position.x.ToRoundedInt();
			bgPos->yOffset = position.y.ToRoundedInt();
		}

		static ControlRegister::Size GetRegSizeFromTileSize(u8 width, u8 height);

	}  ALIGN(4);
}
