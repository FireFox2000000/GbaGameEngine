#include "Time.h"

const double FRAMERATE = 59.97;	// FPS

Time::Time()
{
}

Time::~Time()
{
}

void Time::Update()
{
	IncFrameCount();
}

double Time::GetDeltaTime()
{
	return 1.0f / FRAMERATE;
}

double Time::GetRealSystemTime()
{
	return m_frameCount / FRAMERATE;
}

