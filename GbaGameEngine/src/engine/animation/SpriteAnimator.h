#pragma once

#include "engine/base/core/stl/List.h"
#include "engine/animation/SpriteAnimation.h"

class Engine;

namespace System
{
	namespace SpriteAnimator
	{
		void Update();
	}
}

namespace Component
{
	class SpriteAnimator
	{
		friend void System::SpriteAnimator::Update();

		const SpriteAnimation* m_currentAnimation = nullptr;

		s32 m_timeToNextFrameMicroSeconds = 0;
		u32 m_frameDtMicroseconds = 0;
		int m_currentFrameIndex = -1;

	public:
		void SetAnimation(const SpriteAnimation* animation);
		u32 FrameCount() { return m_currentAnimation ? m_currentAnimation->keyFrames.Count() : 0; }
	};
}
