#pragma once

// We leave this defined for custom bios calls, used for emulator specific functions
#if	defined	(__thumb__)
#define	GBABiosSystemCall(Number)	 __asm ("SWI	  "#Number"\n" :::  "r0", "r1", "r2", "r3")
#else
#define	GBABiosSystemCall(Number)	 __asm ("SWI	  "#Number"	<< 16\n" :::"r0", "r1", "r2", "r3")
#endif

namespace GBA
{
	namespace Bios
	{
		struct ResetFlags
		{
			bool Clear256KOnBoardWRAM : 1;	// Don't use when returning to WRAM
			bool Clear32KInChipWRAM : 1;	// Excluding last 200h bytes
			bool ClearPalette : 1;
			bool CLearVRAM : 1;
			bool ClearOAM : 1;
			bool ResetSIORegisters : 1;
			bool ResetSoundRegisters : 1;
			bool ResetOtherRegisters : 1;
		};

		// Don't let the compiler optimise seemingly empty bios call statements away
#pragma GCC push_options
#pragma GCC optimize ("O0")

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"

		inline void SoftReset()
		{
			GBABiosSystemCall(0x00);
		}

		inline void RegisterRamReset(ResetFlags flags)
		{
			static_assert(sizeof(flags) == sizeof(unsigned char), "ResetFlags struct layout is not correct");
			GBABiosSystemCall(0x01);
		}

		// Ensure VBlank interrupt is actually enabled before calling this
		inline void VBlankIntrWait()
		{
			GBABiosSystemCall(0x05);
		}

		inline int Div(int num, int denom)
		{
			GBABiosSystemCall(0x06);
			// Apparently this returns 3 numbers?...
		}

		// Same as Div (SWI 06h), but incoming parameters are exchanged, r1/r0 (r0=Denom, r1=number). For compatibility with ARM's library. Slightly slower (3 clock cycles) than SWI 06h
		inline int DivArm(int denom, int num)
		{
			GBABiosSystemCall(0x07);
		}

		inline unsigned short Sqrt(unsigned long val)
		{
			GBABiosSystemCall(0x08);
		}

		inline void HardReset()
		{
			GBABiosSystemCall(0x26);
		}

#pragma GCC diagnostic pop

#pragma GCC pop_options
	}
}
