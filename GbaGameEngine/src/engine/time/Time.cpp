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

	clockMs.SetInitialTimerCount(-0x4000);
	clockMs.SetFrequency(Timers::Cycle_1024);
	clockMs.SetActive(true);

	clockSeconds.SetCascadeMode(true);
	clockSeconds.SetActive(true);
}

void Time::Advance()
{
	IncFrameCount();
}

u32 Time::GetMilliseconds() const
{
	return (m_frameCount * 100000 + 5) / FRAMERATE;		// Scale frame to milliseconds, multiply by 100 due to fixed point FRAMERATE, + 5 for automatic rounding
}

u32 Time::GetDtMicroSeconds() const
{
	return DTMICROSECONDS;
}

u32 Time::GetDtMs() const
{
	return DTMS;
}

float Time::GetDtSecondsf() const
{
	return DTSECONDS;
}

u32 Time::GetSecondsSinceStartup()
{
	return Timers::GetTimer(Timers::SystemClock1).GetCurrentTimerCount();
}

