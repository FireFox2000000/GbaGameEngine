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
		u8 m_index = 0;

		void SetControlRegister(int value, u16 attributeMask) { EditControlRegister() = (EditControlRegister() & ~attributeMask) | (value & attributeMask); }

		u16& EditControlRegister();

	public:
		using tCharacterBaseBlock = TileBlockGroups;

		Background() {}; // Purely to make this class std::initializer_list compliant
		Background(u8 index) : m_index(index) {}

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
			u16* hOffAddr = &EditControlRegister() + 4 + m_index;
			*hOffAddr = position.x.ToRoundedInt();
			*(hOffAddr + 1) = position.y.ToRoundedInt();
		}

	}  ALIGN(4);
}
