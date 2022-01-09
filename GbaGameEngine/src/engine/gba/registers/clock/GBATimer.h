#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"

// Hertz == Cycles per seconds

#define GBA_MAX_TIMERS 4

namespace GBA
{
	class Timers
	{
	public:

		// Configureable based on use-case
		enum TimerId : u8
		{
			Sound0,
			Sound1,
			SystemClock1,
			SystemClock2,
		};

		enum ClockCycle : u8
		{
			Cycle_1,
			Cycle_64,
			Cycle_256,
			Cycle_1024,
		};

		class Timer
		{
			friend class Timers;

			union TimerDataRegister
			{
			private:
				u16 initialTimerCount;	// Write-only

			public:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
				const vu16 currentTimerCount;	// Read-only, directly off hardware, no need to initialise
#pragma GCC diagnostic pop

				void SetInitialTimerCount(u16 value) volatile { initialTimerCount = value; }
			};

			struct TimerControlRegister
			{
				u16 frequency : 2
					, cascadeModeEnabled : 1
					, : 3
					, overflowInterruptEnabled : 1
					, isEnabled : 1
					, : 8
					;
			};

			TimerDataRegister dataRegister;
			TimerControlRegister controlRegister;

		public:
			void SetActive(bool active) volatile;
			void SetCascadeMode(bool enabled) volatile;
			void EnableInterruptOnOverflow(bool enabled) volatile;
			void SetFrequency(ClockCycle cycle) volatile;
			void SetInitialTimerCount(u16 value) volatile;
			vu16 GetCurrentTimerCount() volatile const;
		};

	public:
		using tTimers = Array <Timer, GBA_MAX_TIMERS>;

		static const u32 c_systemFrequency = 0x1000000;
		static volatile Timer& GetTimer(TimerId timerId);
	};
}