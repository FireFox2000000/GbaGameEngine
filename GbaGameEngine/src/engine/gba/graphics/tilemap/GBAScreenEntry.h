#pragma once
#include "engine/base/Typedefs.h"

namespace GBA
{
	namespace Gfx
	{
		class ScreenEntry
		{
			u16 m_data = 0;

		public:
            void SetTileIndex(int index);
            void SetHFlipFlag();
			void SetVFlipFlag();
			void SetPalIndex(int index);

			u16 GetData() const { return m_data; }
		};
	}
}
