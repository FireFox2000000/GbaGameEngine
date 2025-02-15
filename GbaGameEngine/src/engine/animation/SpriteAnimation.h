#pragma once
#include "engine/base/core/stl/List.h"
#include <functional>
#include "engine/base/ecs/Entity.h"

class Sprite;
class SpriteAnimator;

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
