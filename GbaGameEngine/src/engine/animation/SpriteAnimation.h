#pragma once
#include "engine/base/core/stl/List.h"
#include <functional>
#include "engine/base/ecs/Entity.h"

namespace GBA
{
	namespace Gfx
	{
		class Sprite;
	}
}

class SpriteAnimator;

using Sprite = GBA::Gfx::Sprite;

struct SpriteAnimation
{
	struct KeyFrame
	{
		Sprite* sprite = nullptr;
	};

	using AnimationEvent = std::function<void(int, ECS::Entity, SpriteAnimator*)>;

	using KeyFrames = List<KeyFrame>;

	KeyFrames keyFrames;
	AnimationEvent onNewFrameHandler = nullptr;
	u32 frameRate = 0;
};
