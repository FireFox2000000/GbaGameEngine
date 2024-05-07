#pragma once

namespace GBA
{
	enum class ClockFrequency : unsigned char
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
		volatile unsigned short counter;

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

	static_assert(sizeof(TimerControl) == 4, "TimerControl struct malformed");

	// Read/write
	TimerControl (*const ioRegisterTimers)[4] = reinterpret_cast<TimerControl(* const)[4]>(0x4000100);
}
