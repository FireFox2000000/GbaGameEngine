#include "AnimationFactory.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlas.h"

SpriteAnimation AnimationFactory::CreateSpriteAtlasSequencedAnimation(GBA::Gfx::SpriteAtlas * spriteAtlas, u8 spriteIndexStart, u8 spriteIndexEnd, u32 framerate)
{
	SpriteAnimation newAnim;
	const int maxFrameCount = spriteIndexEnd - spriteIndexStart;
	newAnim.keyFrames.Reserve(maxFrameCount);

	for (int i = 0; i < maxFrameCount; ++i)
	{
		Sprite* sprite = spriteAtlas->GetSprite(spriteIndexStart + i);
		SpriteAnimation::KeyFrame* keyframe = newAnim.keyFrames.AddNew();
		keyframe->sprite = sprite;
	}

	newAnim.frameRate = framerate;

	return newAnim;
}
