#pragma once
#if defined(PROFILE)

#include "engine/time/Time.h"
#include "engine/debug/DebugLog.h"
#include "engine/gba/config/GBATimerId.h"

#include "gbatek/Timers.h"

class ScopedProfiler
{
	const char* const m_label;
	Time::InternalSnapshot m_profileStart;
public:
	ScopedProfiler(const char* label)
		: m_label(label)
		, m_profileStart(Time::CaptureSystemTimeSnapshot())
	{
	}

	~ScopedProfiler()
	{
		auto profileStop = Time::CaptureSystemTimeSnapshot();
		u32 profileResult = (profileStop.TotalCycles() - m_profileStart.TotalCycles()) * Time::ClockFreq;
		Debug::LogFormat("[%s] = %d", m_label, profileResult);
	}
};

class ScopedProfilerPrecise
{
	static bool s_active;

	const char* const m_label;
public:
	ScopedProfilerPrecise(const char* label, GBATEK::ClockFrequency clockFrequency)
		: m_label(label)
	{
		DEBUG_ASSERTMSG(!s_active, "Started precise profiler when another instance is already active. Only one instance of ScopedProfilerPrecise can be active at a time.");	
		s_active = true;

		auto& profilerClock = (*GBATEK::ioRegisterTimers)[GBATimerId::Profile];
		profilerClock.frequency = clockFrequency;
		profilerClock.isEnabled = true;
	}

	~ScopedProfilerPrecise()
	{
		auto& profilerClock = (*GBATEK::ioRegisterTimers)[GBATimerId::Profile];
		Debug::LogFormat("[%s] = %d", m_label, profilerClock.GetCurrentCount());
		profilerClock.isEnabled = false;
		s_active = false;
	}
};

#define PROFILE_SCOPED(label) auto __scopeProfiler##label = ScopedProfiler(#label)
#define PROFILE_SCOPED_CLOCK_1(label) auto __scopeProfiler##label = ScopedProfilerPrecise(#label, GBATEK::ClockFrequency::Cycle_1)
#define PROFILE_SCOPED_CLOCK_64(label) auto __scopeProfiler##label = ScopedProfilerPrecise(#label, GBATEK::ClockFrequency::Cycle_64)

#define PROFILE_START(label) \
		auto __profileStart##label = Time::CaptureSystemTimeSnapshot()

#define PROFILE_STOP(label) \
{ \
	auto __profileStop##label = Time::CaptureSystemTimeSnapshot(); \
	u32 profileResult = (__profileStop##label.TotalCycles() - __profileStart##label.TotalCycles()) * Time::ClockFreq; \
	Debug::LogFormat("[%s] = %d", #label, profileResult); \
}

#else

#define PROFILE_SCOPED(label)
#define PROFILE_SCOPED_CLOCK_1(label)
#define PROFILE_SCOPED_CLOCK_64(label)
#define PROFILE_START
#define PROFILE_STOP

#endif