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
		u16 m_control;
		s16 m_horizontalOffset;
		s16 m_verticalOffset;

		void SetControlRegister(int value, u16 attributeMask) { m_control = (m_control & ~attributeMask) | (value & attributeMask); }

	public:
		using tCharacterBaseBlock = TileBlockGroups;

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

			AFF_16x16 = 0,
			AFF_32x32 = 1,
			AFF_64x64 = 2,
			AFF_128x128 = 3,
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

		template<class IntType, u8 BITS>
		inline void SetPosition(const Vector2<FixedPoint<IntType, BITS> >& position) // Position of the screen on the map
		{
			m_horizontalOffset = position.x.ToRoundedInt();
			m_verticalOffset = position.y.ToRoundedInt();
		}

	}  ALIGN(4);
}
