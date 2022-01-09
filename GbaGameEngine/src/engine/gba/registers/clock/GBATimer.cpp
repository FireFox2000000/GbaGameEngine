#include "GBATimer.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/base/Macros.h"
#include "engine/math/Math.h"

namespace GBA
{
	void Timers::Timer::SetActive(bool active) volatile
	{
		controlRegister.isEnabled = active;
	}

	void Timers::Timer::SetCascadeMode(bool enabled) volatile
	{
		controlRegister.cascadeModeEnabled = enabled;
	}

	void Timers::Timer::EnableInterruptOnOverflow(bool enabled) volatile
	{
		controlRegister.overflowInterruptEnabled = enabled;
	}

	void Timers::Timer::SetFrequency(ClockCycle cycle) volatile
	{
		controlRegister.frequency = cycle;
	}

	void Timers::Timer::SetInitialTimerCount(u16 value) volatile
	{
		dataRegister.SetInitialTimerCount(value);
	}

	vu16 Timers::Timer::GetCurrentTimerCount() volatile const
	{
		return dataRegister.currentTimerCount;
	}

	volatile Timers::Timer & Timers::GetTimer(TimerId timerId)
	{
		Array<volatile GBA::Timers::Timer, GBA_MAX_TIMERS>& timerRegisters = *reinterpret_cast<Array<volatile GBA::Timers::Timer, GBA_MAX_TIMERS>*>(REG_TIMERDATA);
		return timerRegisters[timerId];
	}
}
