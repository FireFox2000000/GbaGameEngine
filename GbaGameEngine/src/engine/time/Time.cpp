#include "Time.h"
#include "engine/gba/registers/clock/GBATimer.h"

using namespace GBA;

const u32 FRAMERATE = 5997;// FPS, .2f
constexpr u32 DTMICROSECONDS = 100000000 / FRAMERATE;
constexpr u32 DTMS = DTMICROSECONDS / 1000;
const float FPS_FIXED_SCALE = 0.01f;
constexpr float DTSECONDS = 100.f / (float)FRAMERATE;

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

	clockMs.SetInitialTimerCount(-MS_TIMER_START);
	clockMs.SetFrequency(Timers::Cycle_1024);
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

TimeValue Time::GetTimeSinceStartup() const volatile
{
	constexpr u16 SysClock1StartTicks = u16(-Time::MS_TIMER_START);
	const int timeFactor = 1000000;
	const u32 u32Max = 0xffffffff;
	constexpr u32 overflowThreshold = u32Max / timeFactor;

	u32 deltaTicks = Timers::GetTimer(Timers::SystemClock1).GetCurrentTimerCount() - SysClock1StartTicks;
	int overflowCount = 0;
	while (deltaTicks > overflowThreshold)
	{
		deltaTicks -= overflowThreshold;
		++overflowCount;
	}

	u32 microSeconds = (deltaTicks * timeFactor) / Time::MS_TIMER_START + (overflowCount * (u32Max / Time::MS_TIMER_START));
	u32 seconds = Timers::GetTimer(Timers::SystemClock2).GetCurrentTimerCount();

	TimeValue time(microSeconds, seconds);
	return time;
}
