#pragma once
#include "engine/base/Typedefs.h"

#if	defined	( __thumb__ )
#define	BiosSystemCall(Number)	 __asm ("SWI	  "#Number"\n" :::  "r0", "r1", "r2", "r3")
#else
#define	BiosSystemCall(Number)	 __asm ("SWI	  "#Number"	<< 16\n" :::"r0", "r1", "r2", "r3")
#endif

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
