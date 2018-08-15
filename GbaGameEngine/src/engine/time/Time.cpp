#include "Time.h"

const float FRAMERATE = 59.97f;// / 2.0;	// FPS

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

float Time::GetDeltaTime()
{
	return 1.0f / FRAMERATE;
}

float Time::GetRealSystemTime()
{
	return m_frameCount / FRAMERATE;
}

