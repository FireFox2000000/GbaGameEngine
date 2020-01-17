#pragma once
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/base/core/stl/Array.h"

struct SpriteAnimation;

namespace Component
{
	struct RpgMovement
	{
		enum Direction
		{
			None,
			Up,
			Down,
			Left,
			Right,

			Count
		};

		tFixedPoint24 speed = 1.0;
		Direction currentDirection = Direction::None;

		Array<const SpriteAnimation*, Direction::Count> movementAnimations;

		RpgMovement();
	};
}

class Engine;
namespace System
{
	namespace RpgMovement
	{
		void Update(Engine* engine);
		void UpdatePlayerMovement(Engine* engine);
	}
}
