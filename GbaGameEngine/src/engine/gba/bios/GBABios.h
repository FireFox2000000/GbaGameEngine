#pragma once
#include "engine/base/Typedefs.h"

namespace GBA
{
	namespace Bios
	{
		void VBlankIntrWait();
		int Div(int num, int denom);
		void SoftReset();

		u16 Sqrt(u32 val);
	}
}
