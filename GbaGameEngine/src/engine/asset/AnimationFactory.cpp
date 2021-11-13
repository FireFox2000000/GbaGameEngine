#include "AnimationFactory.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"

SpriteAnimation AnimationFactory::CreateSpriteAtlusSequencedAnimation(GBA::Gfx::SpriteAtlus * spriteAtlus, u8 spriteIndexStart, u8 spriteIndexEnd, u32 framerate)
{
	SpriteAnimation newAnim;
	const int maxFrameCount = spriteIndexEnd - spriteIndexStart;
	newAnim.keyFrames.Reserve(maxFrameCount);

	for (int i = 0; i < maxFrameCount; ++i)
	{
		Sprite* sprite = spriteAtlus->GetSprite(spriteIndexStart + i);
		SpriteAnimation::KeyFrame* keyframe = newAnim.keyFrames.AddNew();
		keyframe->sprite = sprite;
	}

	newAnim.frameRate = framerate;

	return newAnim;
}
