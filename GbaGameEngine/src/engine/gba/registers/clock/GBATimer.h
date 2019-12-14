#pragma once
#include "engine/base/Typedefs.h"

// Hertz == Cycles per seconds

namespace GBA
{
	class Timer
	{
	public:
		enum ClockCycle : u8
		{
			Cycle_1,
			Cycle_64,
			Cycle_256,
			Cycle_1024,
		};

		static const u32 c_systemFrequency = 1024 * 0x4000;

		Timer(u8 timerIndex);
		void SetActive(bool active);
		void SetCascadeMode(bool enabled);
		void EnableInterruptOnOverflow(bool enabled);
		void SetFrequency(ClockCycle cycle);
		void SetInitialTimerCount(u16 value);
		u16 GetCurrentTimerCount() const;	
		u8 GetTimerIndex() const { return m_timerIndex; }

	private:
		const u8 m_timerIndex = 0;

		vu16* GetTimerControlAddr() const;
	};
}