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
		Debug::LogFormat("[%s] = %d cycles", m_label, profileResult);
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

		volatile auto& profilerClock = (*GBATEK::ioRegisterTimers)[GBATimerId::Profile];
		profilerClock.frequency = clockFrequency;
		profilerClock.isEnabled = true;
	}

	~ScopedProfilerPrecise()
	{
		volatile auto& profilerClock = (*GBATEK::ioRegisterTimers)[GBATimerId::Profile];
		auto currentCount = profilerClock.GetCurrentCount();
		profilerClock.isEnabled = false;

		switch (profilerClock.frequency)
		{
		case GBATEK::ClockFrequency::Cycle_64:
		{
			currentCount *= 64;
			break;
		}
		case GBATEK::ClockFrequency::Cycle_256:
		{
			currentCount *= 256;
			break;
		}
		case GBATEK::ClockFrequency::Cycle_1024:
		{
			currentCount *= 1024;
			break;
		}
		default: break;
		};

		Debug::LogFormat("[%s] = %hu cycles", m_label, currentCount);
		s_active = false;
	}
};

#define PROFILE_SCOPED(label) auto __scopeProfiler##label = ScopedProfiler(#label)
#define PROFILE_SCOPED_CLOCK_1(label) auto __scopeProfiler##label = ScopedProfilerPrecise(#label, GBATEK::ClockFrequency::Cycle_1)
#define PROFILE_SCOPED_CLOCK_64(label) auto __scopeProfiler##label = ScopedProfilerPrecise(#label, GBATEK::ClockFrequency::Cycle_64)

#define PROFILE_START(label) auto __profileStart##label = Time::CaptureSystemTimeSnapshot()

#define PROFILE_STOP(label) \
{ \
	auto __profileStop##label = Time::CaptureSystemTimeSnapshot(); \
	u32 profileResult = (__profileStop##label.TotalCycles() - __profileStart##label.TotalCycles()) * Time::ClockFreq; \
	Debug::LogFormat("[%s] = %lu cycles", #label, profileResult); \
}

#define PROFILE_START_CLOCK_1(label) \
	auto& profilerClock = (*GBATEK::ioRegisterTimers)[GBATimerId::Profile]; \
	profilerClock.frequency = GBATEK::ClockFrequency::Cycle_1; \
	profilerClock.isEnabled = true; \

#define PROFILE_STOP_CLOCK_1(label) \
{ \
	auto& __profilerClock##label = (*GBATEK::ioRegisterTimers)[GBATimerId::Profile]; \
	u16 currentCount = __profilerClock##label.GetCurrentCount();	\
	__profilerClock##label.isEnabled = false; \
	Debug::LogFormat("[%s] = %hu cycles", #label, currentCount); \
}

#define PROFILE_START_CLOCK_64(label) \
{\
	auto& __profilerClock##label = (*GBATEK::ioRegisterTimers)[GBATimerId::Profile]; \
	__profilerClock##label.frequency = GBATEK::ClockFrequency::Cycle_64; \
	__profilerClock##label.isEnabled = true; \
}

#define PROFILE_STOP_CLOCK_64(label) \
{ \
	auto& __profilerClock##label = (*GBATEK::ioRegisterTimers)[GBATimerId::Profile]; \
	u16 currentCount = __profilerClock##label.GetCurrentCount();	\
	__profilerClock##label.isEnabled = false; \
	Debug::LogFormat("[%s] = %hu cycles", #label, currentCount * 64); \
}

#else

#define PROFILE_SCOPED(label)
#define PROFILE_SCOPED_CLOCK_1(label)
#define PROFILE_SCOPED_CLOCK_64(label)
#define PROFILE_START
#define PROFILE_STOP

#endif