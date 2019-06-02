#pragma once

#include "engine/base/core/stl/List.h"

class SpriteRenderer;
class Sprite;
class Engine;

struct SpriteAnimation
{
	struct KeyFrame
	{
		Sprite* sprite;
	};

	typedef List<KeyFrame> KeyFrames;

	KeyFrames keyFrames;
	u32 frameRate;
};

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

		SpriteAnimation currentAnimation;

		s32 timeToNextFrameMicroSeconds = 0;
		u32 frameDtMicroseconds = 0;
		u8 currentFrameIndex = 0;

	public:
		void SetAnimation(const SpriteAnimation& animation);
		u32 FrameCount() { return currentAnimation.keyFrames.Count(); }
	};
}
