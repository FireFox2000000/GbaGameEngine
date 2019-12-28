#pragma once
#include "engine/base/core/stl/FixedPoint.h"

class Engine;

namespace Component
{
	struct PlayerMovement
	{
		tFixedPoint24 moveSpeed = 8.0f;
	};
}

namespace System
{
	namespace PlayerMovement
	{
		void Update(Engine* engine);
	}
}
