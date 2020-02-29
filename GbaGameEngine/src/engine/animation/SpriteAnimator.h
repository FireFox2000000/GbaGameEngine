#pragma once

#include "engine/base/core/stl/List.h"
#include "engine/animation/SpriteAnimation.h"

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
		int currentFrameIndex = -1;

	public:
		void SetAnimation(const SpriteAnimation* animation);
		u32 FrameCount() { return currentAnimation ? currentAnimation->keyFrames.Count() : 0; }
	};
}
