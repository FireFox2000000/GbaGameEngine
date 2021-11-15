#include "Time.h"
#include "engine/gba/registers/clock/GBATimer.h"

using namespace GBA;

constexpr u16 MS_TIMER_START = 0x0;
constexpr u16 SysClock1StartTicks = u16(-MS_TIMER_START);

#define U16_MAX u16(-1)
constexpr u32 CyclesPerSecond = U16_MAX - SysClock1StartTicks;

Time::Time()
{
}

Time::~Time()
{
}

void Time::Start()
{
	// Overflow every ~1 second:
	// 0x4000 ticks @ FREQ_1024

	Timers::Timer& clockMs = Timers::GetTimer(Timers::SystemClock1);
	Timers::Timer& clockSeconds = Timers::GetTimer(Timers::SystemClock2);

	clockMs.SetInitialTimerCount(SysClock1StartTicks);
	clockMs.SetFrequency(Timers::Cycle_256);
	clockMs.SetActive(true);

	clockSeconds.SetCascadeMode(true);
	clockSeconds.SetActive(true);
}

void Time::Advance()
{
	IncFrameCount();

	TimeValue currentTime = GetTimeSinceStartup();
	m_dt = currentTime - m_frameStartTime;
	m_frameStartTime = currentTime;
}

TimeValue Time::GetDt() const
{
	return m_dt;
}

TimeValue Time::FromSnapshot(const InternalSnapshot& snapshot)
{
	constexpr int timeFactor = 1000000;
	constexpr u32 u32Max = 0xffffffff;
	constexpr u32 overflowThreshold = u32Max / timeFactor;

	u32 deltaTicks = snapshot.systemClockCount1 - SysClock1StartTicks;
	int overflowCount = 0;
	while (deltaTicks > overflowThreshold)
	{
		deltaTicks -= overflowThreshold;
		++overflowCount;
	}

	constexpr int TotalCyclesPerOverflow = U16_MAX;		// This might actually be off by a tick...
	u32 microSeconds = (deltaTicks * timeFactor) / TotalCyclesPerOverflow + (overflowCount * (u32Max / TotalCyclesPerOverflow));
	u32 seconds = snapshot.systemClockCount2;

	return TimeValue(microSeconds, seconds);
}

TimeValue Time::GetTimeSinceStartup() const volatile
{
	InternalSnapshot snapshot = CaptureSystemTimeSnapshot();
	return FromSnapshot(snapshot);
}

Time::InternalSnapshot Time::CaptureSystemTimeSnapshot()
{
	return { Timers::GetTimer(Timers::SystemClock1).GetCurrentTimerCount(), Timers::GetTimer(Timers::SystemClock2).GetCurrentTimerCount() };
}

u32 Time::InternalSnapshot::TotalCycles() const
{
	u32 clock1Cycles = (systemClockCount1 - SysClock1StartTicks);
	u32 clock2Cycles = systemClockCount2 * CyclesPerSecond;
	return clock1Cycles + clock2Cycles;
}
