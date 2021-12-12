#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/FixedPoint.h"

class TimeValue
{
	u32 m_decimalMicroseconds = 0;
	u32 m_seconds = 0;

public:
	TimeValue() = default;
	TimeValue(u32 decimalMicroSeconds, u32 seconds);

	TimeValue& operator += (const TimeValue& timeValue);
	TimeValue& operator -= (const TimeValue& timeValue);

	inline TimeValue operator+(const TimeValue& timeValue) const { return TimeValue(*this) += timeValue; }
	inline TimeValue operator-(const TimeValue& timeValue) const { return TimeValue(*this) -= timeValue; }

	u32 TotalMicroseconds() const;
	u32 TotalMilliseconds() const;
	u32 TotalSeconds() const;
	u32 TotalMinutes() const;
	u32 TotalHours() const;

	// Returns time in seconds.
	float ToFloat() const;
	tFixedPoint24 ToFp24() const;
};

