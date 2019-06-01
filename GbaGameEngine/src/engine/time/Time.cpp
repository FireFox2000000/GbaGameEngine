#include "Time.h"

const u32 FRAMERATE = 5997;// FPS, .2f
constexpr u32 DTMICROSECONDS = 100000000 / FRAMERATE;
constexpr u32 DTMS = DTMICROSECONDS / 1000;
const float FPS_FIXED_SCALE = 0.01f;

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

u32 Time::GetMilliseconds()
{
	return (m_frameCount * 100000 + 5) / FRAMERATE;		// Scale frame to milliseconds, multiply by 100 due to fixed point FRAMERATE, + 5 for automatic rounding
}

u32 Time::GetDtMicroSeconds()
{
	return DTMICROSECONDS;
}

u32 Time::GetDtMs()
{
	return DTMS;
}

float Time::GetDtSecondsf()
{
	return 100.f / (float)FRAMERATE;
}

float Time::GetRealSystemTimeSecondsf()
{
	return m_frameCount * FPS_FIXED_SCALE / (float)FRAMERATE;
}

