#include "TimeValue.h"
#include "Time.h"
#include "engine/math/Math.h"

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

	m_seconds -= MIN(m_seconds, timeValue.m_seconds);		// Guard against underflow

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
	float microSecondsF = (float)m_decimalMicroseconds / MICROSECONDS_PER_SECOND;
	return m_seconds + microSecondsF;
}
