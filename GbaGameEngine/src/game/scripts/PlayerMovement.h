#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

class Engine;
class GameObject;

namespace Component
{
	struct PlayerMovement
	{
		tFixedPoint24 moveSpeed = 8.0f;
	};
}


namespace PlayerMovement
{
	void MoveHumanPlayerObject(Engine* engine, GameObject& playerObject);
	void MovePlayerObject(Engine* engine, GameObject& playerObject, const Vector2<int>& desiredDirection);
}

