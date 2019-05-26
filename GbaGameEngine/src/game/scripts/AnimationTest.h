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

		u32 m_frameRateMs;
		u32 m_totalFrames;
		KeyFrames m_keyFrames;
		u32 m_startTimeMilliseconds;

		AnimationTest(Engine* engine);
		~AnimationTest();

		void SetFrameRate(u32 fps);
		u32 GetFrameRate();

	};
}

namespace System
{
	namespace AnimationTest
	{
		void Update(Engine* engine);
	}
}

