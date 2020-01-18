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
		bool enableHorizontalDirSpriteFlip = true;

	private:
		Direction currentDirection = Direction::None;
		Direction currentFacing = Direction::None;

	public:
		using tAnimationContainer = Array<const SpriteAnimation*, Direction::Count>;

		tAnimationContainer movementAnimations;
		tAnimationContainer idleAnimations;

		RpgMovement();

		Direction GetCurrentDirection() const;
		Direction GetCurrentFacing() const;
		void SetCurrentDirection(Direction dir);
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
