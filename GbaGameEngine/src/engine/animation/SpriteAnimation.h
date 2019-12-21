#pragma once
#include "engine/base/core/stl/List.h"

class Sprite;

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
