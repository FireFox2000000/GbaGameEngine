#pragma once
#include "array.h"

namespace GBA
{
	enum class ClockFrequency
	{
		Cycle_1,
		Cycle_64,
		Cycle_256,
		Cycle_1024,
	};

	struct TimerControl
	{
	private:
		// Writing to this value sets the initial timer count, it does not set the current timer value
		// Reading from this value returns the current timer count
		unsigned short counter;

	public:
		ClockFrequency frequency : 2;
		bool cascadeModeEnabled : 1
			, : 3;	// unused
		bool overflowInterruptEnabled : 1;
		bool isEnabled : 1
			, : 8	// unused
			;

		inline void SetInitialCount(unsigned short value) volatile { counter = value; }
		inline unsigned short GetCurrentCount() const volatile { return counter; }
	};

	// Read/write
	array<TimerControl, 4>* const ioRegisterTimers = reinterpret_cast<array<TimerControl, 4>*>(0x4000100);
}
