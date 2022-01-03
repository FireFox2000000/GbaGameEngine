#pragma once

namespace GBA
{
	namespace Bios
	{
		void VBlankIntrWait();
		int Div(int num, int denom);
		void SoftReset();
	}
}
