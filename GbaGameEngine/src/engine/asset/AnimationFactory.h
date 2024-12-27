#pragma once
#include "engine/animation/SpriteAnimation.h"

namespace GBA
{
	namespace Gfx
	{
		class SpriteAtlas;
	}
}

namespace AnimationFactory
{
	SpriteAnimation CreateSpriteAtlasSequencedAnimation(GBA::Gfx::SpriteAtlas* spriteAtlas, u8 spriteIndexStart, u8 spriteIndexEnd, u32 framerate);
}
