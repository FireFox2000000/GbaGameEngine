#pragma once
#include "Interrupts.h"
#include "internal/Internal.h"

namespace GBATEK
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
GBATEK_OPTIONS_PUSH_OPTIMIZE_O0
GBATEK_DIAGNOSTIC_PUSH_IGNORED_MISSING_RETURN

		inline void SoftReset()
		{
			GBATEK_BiosSystemCall(0x00);
		}

		inline void RegisterRamReset(ResetFlags flags)
		{
			static_assert(sizeof(flags) == sizeof(unsigned char), "ResetFlags struct layout is not correct");
			GBATEK_BiosSystemCall(0x01);
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
			GBATEK_BiosSystemCall(0x04);
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
			GBATEK_BiosSystemCall(0x05);
		}

		inline int Div(int num, int denom)
		{
			GBATEK_BiosSystemCall(0x06);
			// Apparently this returns 3 numbers?...
			// r0  Number DIV Denom; signed
			// r1  Number MOD Denom; signed
			// r3  ABS(Number DIV Denom); unsigned
			// For example, incoming -1234, 10 should return -123, -4, +123.
			// TODO implement - https://github.com/yoyz/gba/blob/6b171695033ec2ea486c495e737c2e394d40b7d2/tonclib/code/tonclib/asm/tonc_bios_ex.s#L55
		}

		// Same as Div (SWI 06h), but incoming parameters are exchanged, r1/r0 (r0=Denom, r1=number). For compatibility with ARM's library. Slightly slower (3 clock cycles) than SWI 06h
		inline int DivArm(int denom, int num)
		{
			GBATEK_BiosSystemCall(0x07);
		}

		inline unsigned short Sqrt(unsigned long val)
		{
			GBATEK_BiosSystemCall(0x08);
		}

		inline void HardReset()
		{
			GBATEK_BiosSystemCall(0x26);
		}

GBATEK_DIAGNOSTIC_POP
GBATEK_OPTIONS_POP
	}
}
