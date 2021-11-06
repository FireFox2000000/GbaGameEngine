#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

struct Matrix2x2
{
	using FP8 = FixedPoint<s16, 8>;

	FP8 a;
	FP8 b;
	FP8 c;
	FP8 d;

	// rotationAlpha range is [0, 0xFFFF] for [0, 2 Pi]
	void SetTransformation(Vector2<tFixedPoint8> scale, u16 rotationAlpha);
};
