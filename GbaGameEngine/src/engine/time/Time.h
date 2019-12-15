#pragma once

#include "engine/base/Typedefs.h"

class Time
{
	u32 m_frameCount = 0;

	void IncFrameCount() { ++m_frameCount; }

public:
	Time();
	~Time();

	void Start();
	void Advance();

	u32 GetFrameCount() { return m_frameCount; }
	u32 GetMilliseconds() const;
	u32 GetDtMicroSeconds() const;
	u32 GetDtMs() const;

	float GetDtSecondsf() const;

	u32 GetSecondsSinceStartup();
};
