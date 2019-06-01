#pragma once

#include "engine/base/Typedefs.h"

class Time
{
	u32 m_frameCount = 0;

	void IncFrameCount() { ++m_frameCount; }

public:
	Time();
	~Time();

	void Update();

	u32 GetFrameCount() { return m_frameCount; }
	u32 GetMilliseconds();
	u32 GetDtMicroSeconds();
	u32 GetDtMs();
	float GetDtSecondsf();
	float GetTimeSecondsf() { return GetRealSystemTimeSecondsf(); }	// TODO, don't actually do this, can't scale time.
	float GetRealSystemTimeSecondsf();
};
