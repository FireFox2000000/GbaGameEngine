#ifndef PRAGMA_ONCE_ENGINE_TIME_TIME_H
#define PRAGMA_ONCE_ENGINE_TIME_TIME_H

#include "engine/base/Typedefs.h"

class Time
{
	u32 m_frameCount;

	void IncFrameCount() { ++m_frameCount; }

public:
	Time();
	~Time();

	void Update();

	u32 GetFrameCount() { return m_frameCount; }
	float GetDeltaTime();
	float GetTime() { return GetRealSystemTime(); }	// TODO, don't actually do this, can't scale time.
	float GetRealSystemTime();
};

#endif
