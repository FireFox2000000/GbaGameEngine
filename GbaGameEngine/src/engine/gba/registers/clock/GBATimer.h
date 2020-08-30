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
			SystemClock1,
			SystemClock2,
			Profile,
			Unused,
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

		public:
			void SetActive(bool active);
			void SetCascadeMode(bool enabled);
			void EnableInterruptOnOverflow(bool enabled);
			void SetFrequency(ClockCycle cycle);
			void SetInitialTimerCount(u16 value);
			s16 GetCurrentTimerCount() const;
			u8 GetTimerIndex() const { return m_timerIndex; }

			Timer() {}; // Purely to make this class std::initializer_list compliant
			
		private:
			Timer(u8 timerIndex);

			u8 m_timerIndex = 0;

			vu16* GetTimerControlAddr() const;
		};

	private:
		using tTimers = Array <Timer, GBA_MAX_TIMERS>;
		static tTimers s_timers;

	public:
		static const u32 c_systemFrequency = 0x1000000;
		static Timer& GetTimer(TimerId timerId);
	};
}