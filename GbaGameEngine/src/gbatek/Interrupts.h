#pragma once
namespace GBATEK
{
	enum class InterruptFlags : short
	{
		None = 0,

		VBlank = 1 << 0,
		HBlank = 1 << 1,
		VCount = 1 << 2,
		Timer0Overflow = 1 << 3,
		Timer1Overflow = 1 << 4,
		Timer2Overflow = 1 << 5,
		Timer3Overflow = 1 << 6,
		SerialCommunication = 1 << 7,
		Dma0 = 1 << 8,
		Dma1 = 1 << 9,
		Dma2 = 1 << 10,
		Dma3 = 1 << 11,
		Keypad = 1 << 12,
		GamepakRemoved = 1 << 13,
	};

	inline constexpr InterruptFlags operator|(InterruptFlags a, InterruptFlags b)
	{
		return static_cast<InterruptFlags>(static_cast<short>(a) | static_cast<short>(b));
	}

	inline constexpr InterruptFlags operator&(InterruptFlags a, InterruptFlags b)
	{
		return static_cast<InterruptFlags>(static_cast<short>(a) & static_cast<short>(b));
	}

	inline constexpr InterruptFlags& operator|=(InterruptFlags& a, InterruptFlags b)
	{
		return a = a | b;
	}

	inline constexpr InterruptFlags& operator&=(InterruptFlags& a, InterruptFlags b)
	{
		return a = a & b;
	}

	struct InterruptRequestFlagsRegister
	{
		const InterruptFlags interrupts;

		inline void AcknowledgeInterrupts(InterruptFlags interrupts) volatile
		{
			// This may look like we're stomping over the active flags here, but that's not the case. 
			// Special GBA magic happens at this address where writing to it only acknowledges
			// the bits we're writing with. 
			*const_cast<InterruptFlags*>(&this->interrupts) = interrupts;
		}
	};

	struct BiosInterruptRequestFlagsRegister
	{
		const InterruptFlags interrupts;

		inline void AcknowledgeInterrupts(InterruptFlags interrupts)
		{
			// When using InterruptWait, the user interrupt handler MUST update 
			// the BIOS Interrupt Flags value in RAM when acknowleding processed interrupt(s),
			// by writing a value to the IF register (GBA::ioRegisterBiosInterruptFlags). 
			// The same value should be also ORed to the BIOS Interrupt Flags value, 
			// at this register
			*const_cast<InterruptFlags*>(&this->interrupts) |= interrupts;
		}
	};

	struct InterruptMasterEnable
	{
		enum class Status : unsigned short
		{
			DisableAll,
			InterruptEnableRegister
		};

		Status status;	// Only the first bit is actually used here. 
	};

	static_assert(sizeof(InterruptFlags) == 2, "Interrupts struct malformed");
	static_assert(sizeof(InterruptRequestFlagsRegister) == 2, "InterruptRequestFlagsRegister struct malformed");
	static_assert(sizeof(BiosInterruptRequestFlagsRegister) == 2, "BiosInterruptRequestFlagsRegister struct malformed");
	static_assert(sizeof(InterruptMasterEnable) == 2, "InterruptMasterEnable struct malformed");

	using InterruptHandlerFnPtr = void (*)(void);

	// Note that the code pointed to here must be ARM code
	// Mark your function with GBATEK_ARM_IWRAM_CODE from gbatek/GBA.h to be compatible
	InterruptHandlerFnPtr* const ioRegisterInterruptHandler = reinterpret_cast<InterruptHandlerFnPtr*>(0x03007FFC);
	
	InterruptFlags* const ioRegisterInterruptEnable = reinterpret_cast<InterruptFlags*>(0x4000200);

	// Flags that an interrupt has occurred
	volatile InterruptRequestFlagsRegister* const ioRegisterInterruptRequestFlags = reinterpret_cast<volatile InterruptRequestFlagsRegister*>(0x4000202);
	InterruptMasterEnable* const ioRegisterInterruptMasterEnable = reinterpret_cast<InterruptMasterEnable*>(0x4000208);

	// When using IntrWait or VBlankIntrWait, the user interrupt handler MUST update 
	// the BIOS Interrupt Flags value in RAM when acknowleding processed interrupt(s),
	// by writing a value to the IF register. 
	// The same value should be also ORed to the BIOS Interrupt Flags value, 
	// at this register
	BiosInterruptRequestFlagsRegister* const ioRegisterBiosInterruptFlags = reinterpret_cast<BiosInterruptRequestFlagsRegister*>(0x3007FF8);
}
