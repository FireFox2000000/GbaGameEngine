#pragma once
#include "GBASDK/Interrupts.h"

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
			bool clear256KOnBoardWRAM : 1;	// Don't use when returning to WRAM
			bool clear32KInChipWRAM : 1;	// Excluding last 200h bytes
			bool clearPalette : 1;
			bool cLearVRAM : 1;
			bool clearOAM : 1;
			bool resetSIORegisters : 1;
			bool resetSoundRegisters : 1;
			bool resetOtherRegisters : 1;
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

		/// <summary>
		// When using InterruptWait, the user interrupt handler MUST update 
		// the BIOS Interrupt Flags value in RAM when acknowleding processed interrupt(s),
		// by writing a value to the IF register (GBA::ioRegisterBiosInterruptFlags). 
		// The same value should be also ORed to the BIOS Interrupt Flags value, 
		// at this register
		/// </summary>
		// <param name="waitUntilNew">
		// false = Return immediately if an old flag was already set (NDS9: bugged!)
		// true = Discard old flags, wait until a NEW flag becomes set
		// </param>
		inline void InterruptWait(bool waitUntilNew, Interrupts interrupts)
		{
			GBABiosSystemCall(0x04);
		}
		 
		/// <summary>
		// Ensure VBlank interrupt is actually enabled before calling this.
		// When using VBlankInterruptWait, the user interrupt handler MUST update 
		// the BIOS Interrupt Flags value in RAM when acknowleding processed interrupt(s),
		// by writing a value to the IF register (GBA::ioRegisterBiosInterruptFlags). 
		// The same value should be also ORed to the BIOS Interrupt Flags value, 
		// at this register
		/// </summary>
		inline void VBlankInterruptWait()
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
