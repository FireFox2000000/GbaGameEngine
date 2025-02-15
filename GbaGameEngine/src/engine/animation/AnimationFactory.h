#pragma once
#include "engine/animation/SpriteAnimation.h"

class SpriteAtlas;

namespace AnimationFactory
{
	SpriteAnimation CreateSpriteAtlasSequencedAnimation(SpriteAtlas* spriteAtlas, u8 spriteIndexStart, u8 spriteIndexEnd, u32 framerate);
}
