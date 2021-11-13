#pragma once
#include "engine/animation/SpriteAnimation.h"

namespace GBA
{
	namespace Gfx
	{
		class SpriteAtlus;
	}
}

namespace AnimationFactory
{
	SpriteAnimation CreateSpriteAtlusSequencedAnimation(GBA::Gfx::SpriteAtlus* spriteAtlus, u8 spriteIndexStart, u8 spriteIndexEnd, u32 framerate);
}
