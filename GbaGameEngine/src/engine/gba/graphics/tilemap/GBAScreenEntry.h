#pragma once
#include "engine/base/Typedefs.h"

namespace GBA
{
	namespace Gfx
	{
		class ScreenEntry
		{
			u16 m_tileIndex : 10
				, m_hFlip : 1
				, m_vFlip : 1
				, m_palIndex : 4
				;

		public:
            void SetTileIndex(int index);
            void SetHFlipFlag();
			void SetVFlipFlag();
			void SetPalIndex(int index);
		};
	}
}
