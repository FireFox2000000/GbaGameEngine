#pragma once

#include "engine/base/Typedefs.h"
#include "TimeValue.h"

class Time
{
	u32 m_frameCount = 0;
	TimeValue m_frameStartTime;
	TimeValue m_dt;
	void IncFrameCount() { ++m_frameCount; }

public:
	static constexpr int ClockFreq = 256;

	// A cheap way to calculate system time in internal format
	struct InternalSnapshot
	{
		u16 systemClockCount1;
		u16 systemClockCount2;

		u32 TotalCycles() const;
	};

	Time();
	~Time();

	void Start();
	void Advance();

	u32 GetFrameCount() { return m_frameCount; }
	TimeValue GetDt() const;
	static TimeValue FromSnapshot(const InternalSnapshot& snapshot);
	TimeValue GetTimeSinceStartup() const volatile;
	static InternalSnapshot CaptureSystemTimeSnapshot();
};
