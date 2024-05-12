#pragma once
#include "GBASDK/Interrupts.h"
#include "internal/Internal.h"

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

		static_assert(sizeof(ResetFlags) == 1, "ResetFlags struct malformed");

		// Don't let the compiler optimise seemingly empty bios call statements away
GBA_OPTIONS_PUSH_OPTIMIZE_O0
GBA_DIAGNOSTIC_PUSH_IGNORED_MISSING_RETURN

		inline void SoftReset()
		{
			GBA_BiosSystemCall(0x00);
		}

		inline void RegisterRamReset(ResetFlags flags)
		{
			static_assert(sizeof(flags) == sizeof(unsigned char), "ResetFlags struct layout is not correct");
			GBA_BiosSystemCall(0x01);
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
			GBA_BiosSystemCall(0x04);
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
			GBA_BiosSystemCall(0x05);
		}

		inline int Div(int num, int denom)
		{
			GBA_BiosSystemCall(0x06);
			// Apparently this returns 3 numbers?...
		}

		// Same as Div (SWI 06h), but incoming parameters are exchanged, r1/r0 (r0=Denom, r1=number). For compatibility with ARM's library. Slightly slower (3 clock cycles) than SWI 06h
		inline int DivArm(int denom, int num)
		{
			GBA_BiosSystemCall(0x07);
		}

		inline unsigned short Sqrt(unsigned long val)
		{
			GBA_BiosSystemCall(0x08);
		}

		inline void HardReset()
		{
			GBA_BiosSystemCall(0x26);
		}

GBA_DIAGNOSTIC_POP
GBA_OPTIONS_POP
	}
}
