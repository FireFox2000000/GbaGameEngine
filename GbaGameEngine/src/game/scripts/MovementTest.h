#pragma once
#include "engine/component/MonoBehaviour.h"
#include "engine/base/core/stl/FixedPoint.h"

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
