#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Vector2.h"

class GameObject;

struct PlayerMovement
{
	FPi16 moveSpeed = 8.0f;
	FPi16 jumpInitVel = 15.f;

	static void MoveHumanPlayerObject(GameObject& playerObject);
	static void MovePlayerObject(GameObject& playerObject, const Vector2i& desiredDirection);
};
