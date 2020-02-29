#pragma once
#include "engine/base/core/stl/List.h"

namespace GBA
{
	namespace Gfx
	{
		class Sprite;
	}
}

using Sprite = GBA::Gfx::Sprite;

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
