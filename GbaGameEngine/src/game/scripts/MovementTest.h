#pragma once
#include "engine/component/MonoBehaviour.h"

namespace Component
{
	struct PlayerMovement
	{
		float moveSpeed = 8.0f;
	};
}

namespace System
{
	namespace PlayerMovement
	{
		void Update(Engine* engine);
	}
}
