#pragma once

#include "engine/base/core/stl/List.h"

class SpriteRenderer;
class Sprite;
class Engine;

namespace Component
{
	struct AnimationTest
	{
		struct KeyFrame
		{
			Sprite* sprite;
		};

		typedef List<KeyFrame> KeyFrames;

		s32 m_timeToNextFrameMicroSeconds = 0;
		u32 m_frameDtMicroseconds = 0;
		u8 m_currentFrameIndex = 0;

		KeyFrames m_keyFrames;

		AnimationTest(Engine* engine);
		~AnimationTest();

		void SetFrameRate(u32 fps);
		u32 FrameCount() { return m_keyFrames.Count(); }
	};
}

namespace System
{
	namespace AnimationTest
	{
		void Update(Engine* engine);
	}
}

