#pragma once
namespace GBA
{
	struct Interrupts
	{
		union
		{
			// Disable anonymous struct warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
			struct
			{
				bool vBlank : 1;
				bool hBlank : 1;
				bool vCount : 1;
				bool timer0Overflow : 1;
				bool timer1Overflow : 1;
				bool timer2Overflow : 1;
				bool timer3Overflow : 1;
				bool serialCommunication : 1;
				bool dma0 : 1;
				bool dma1 : 1;
				bool dma2 : 1;
				bool dma3 : 1;
				bool keypad : 1;
				bool gamepakRemoved : 1,
					: 2;	// unused
			};
#pragma GCC diagnostic pop

			unsigned short data;
		};
	};

	struct InterruptRequestFlagsRegister : Interrupts
	{
		inline void AcknowledgeInterrupts(Interrupts interrupts) volatile
		{
			// This may look like we're stomping over the active flags here, but that's not the case. 
			// Special GBA magic happens at this address where writing to it only acknowledges
			// the bits we're writing with. 
			data = interrupts.data;
		}
	};

	struct BiosInterruptRequestFlagsRegister : Interrupts
	{
		inline void AcknowledgeInterrupts(Interrupts interrupts)
		{
			// When using InterruptWait, the user interrupt handler MUST update 
			// the BIOS Interrupt Flags value in RAM when acknowleding processed interrupt(s),
			// by writing a value to the IF register (GBA::ioRegisterBiosInterruptFlags). 
			// The same value should be also ORed to the BIOS Interrupt Flags value, 
			// at this register
			data |= interrupts.data;
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

	using InterruptHandlerFnPtr = void (*)(void);

	// Note that the code pointed to here must be ARM code
	// Mark your function with GBA_ARM_IWRAM_CODE from GBASDK/GBA.h to be compatible
	InterruptHandlerFnPtr* const ioRegisterInterruptHandler = reinterpret_cast<InterruptHandlerFnPtr*>(0x03007FFC);
	
	Interrupts* const ioRegisterInterruptEnable = reinterpret_cast<Interrupts*>(0x4000200);

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
