#pragma once

#include "engine/base/Typedefs.h"

class Time
{
	u32 m_frameCount = 0;

	void IncFrameCount() { ++m_frameCount; }

public:
	Time();
	~Time();

	void Advance();

	u32 GetFrameCount() { return m_frameCount; }
	u32 GetMilliseconds() const;
	u32 GetDtMicroSeconds() const;
	u32 GetDtMs() const;
	float GetDtSecondsf() const;
	float GetTimeSecondsf() const { return GetRealSystemTimeSecondsf(); }	// TODO, don't actually do this, can't scale time.
	float GetRealSystemTimeSecondsf() const;
};
