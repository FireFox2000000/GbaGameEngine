#pragma once
#include "engine/base/Typedefs.h"

namespace GBA
{
	namespace Interrupts
	{
		enum InterruptType
		{
			VBlank,
			HBlank,
			VCount,
			Timer0,
			Timer1,
			Timer2,
			Timer3,
			SerialCommunication,
			Dma0,
			Dma1,
			Dma2,
			Dma3,
			Keypad,
			GamepakRemoved,

			Count
		};

		void DisableInterrupts();
		void EnableInterrupts();

		void EnableInterrupt(InterruptType interrupt);
		u16 GetActiveInterrupts();
		bool IsInterruptActive(InterruptType interrupt);
		bool IsInterruptEnabled(InterruptType interrupt);
		void AcknowledgeInterrupt(InterruptType interrupt);
		void AcknowledgeInterrupts(u16 interrupts);
		void AcknowledgeInterruptBios(InterruptType interrupt);

		typedef void (*InterruptHandler)(void);
		void RegisterInterruptHandler(InterruptHandler armHandler);
	};
}
