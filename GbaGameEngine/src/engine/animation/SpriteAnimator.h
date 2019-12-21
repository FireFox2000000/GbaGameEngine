#pragma once

#include "engine/base/core/stl/List.h"
#include "engine/animation/SpriteAnimation.h"

class SpriteRenderer;
class Sprite;
class Engine;

namespace System
{
	class SpriteAnimator
	{
	public:
		static void Update(Engine* engine);
	};
}

namespace Component
{
	class SpriteAnimator
	{
		friend class System::SpriteAnimator;

		const SpriteAnimation* currentAnimation;

		s32 timeToNextFrameMicroSeconds = 0;
		u32 frameDtMicroseconds = 0;
		u8 currentFrameIndex = 0;

	public:
		void SetAnimation(const SpriteAnimation* animation);
		u32 FrameCount() { return currentAnimation->keyFrames.Count(); }
	};
}
