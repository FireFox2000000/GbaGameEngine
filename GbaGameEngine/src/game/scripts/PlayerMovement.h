#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

class Engine;
class GameObject;

namespace Component
{
	struct PlayerMovement
	{
		FPi16 moveSpeed = 8.0f;
		FPi16 jumpInitVel = 15.f;
	};
}


namespace PlayerMovement
{
	void MoveHumanPlayerObject(GameObject& playerObject);
	void MovePlayerObject(GameObject& playerObject, const Vector2i& desiredDirection);
}

