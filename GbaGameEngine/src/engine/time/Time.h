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
	Time();
	~Time();

	static const u16 MS_TIMER_START = 0x4000;

	void Start();
	void Advance();

	u32 GetFrameCount() { return m_frameCount; }
	TimeValue GetDt() const;
	TimeValue GetTimeSinceStartup() const volatile;
};
