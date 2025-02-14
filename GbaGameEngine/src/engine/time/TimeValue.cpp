#include "TimeValue.h"
#include "Time.h"
#include "engine/math/Math.h"

constexpr int MICROSECONDS_PER_SECOND = 1000000;
constexpr float MICROSECONDS_PER_SECOND_INV = 1.0f / MICROSECONDS_PER_SECOND;

TimeValue::TimeValue(u32 decimalMicroSeconds, u32 seconds)
{
	m_decimalMicroseconds = decimalMicroSeconds % MICROSECONDS_PER_SECOND;
	m_seconds = seconds + decimalMicroSeconds / MICROSECONDS_PER_SECOND;
}

TimeValue & TimeValue::operator+=(const TimeValue & timeValue)
{
	m_decimalMicroseconds += timeValue.m_decimalMicroseconds;
	m_seconds += timeValue.m_seconds;

	if (m_decimalMicroseconds > MICROSECONDS_PER_SECOND)
	{
		m_decimalMicroseconds -= MICROSECONDS_PER_SECOND;
		++m_seconds;
	}

	return *this;
}

TimeValue & TimeValue::operator-=(const TimeValue & timeValue)
{
	// Guard against underflow
	if (m_decimalMicroseconds < timeValue.m_decimalMicroseconds)
	{
		m_decimalMicroseconds += MICROSECONDS_PER_SECOND;

		if (m_seconds > 0)
		{
			m_seconds -= 1;
		}
	}

	m_decimalMicroseconds -= timeValue.m_decimalMicroseconds;

	m_seconds -= Math::Min(m_seconds, timeValue.m_seconds);		// Guard against underflow

	return *this;
}

u32 TimeValue::TotalMicroseconds() const
{
	return m_decimalMicroseconds + (m_seconds * MICROSECONDS_PER_SECOND);
}

u32 TimeValue::TotalMilliseconds() const
{
	return m_decimalMicroseconds / 1000 + (m_seconds * 1000);
}

u32 TimeValue::TotalSeconds() const
{
	return m_seconds;
}

u32 TimeValue::TotalMinutes() const
{
	return m_seconds / 60;
}

u32 TimeValue::TotalHours() const
{
	return TotalMinutes() / 60;
}

float TimeValue::ToFloat() const
{
	float microSecondsF = (float)m_decimalMicroseconds * MICROSECONDS_PER_SECOND_INV;
	return m_seconds + microSecondsF;
}

FPi24 TimeValue::ToFp24() const
{
	return FPi24((int)m_seconds) + FPi24((float)m_decimalMicroseconds * MICROSECONDS_PER_SECOND_INV);
}
