#include "AnimationLibrary.h"

AnimationLibrary::AnimationLibrary(SpriteLibrary * spriteLib)
{
	DEBUG_LOG("Setting up Animation Library...");

#define SPRITE_ANIM_ENTRY(Namespace, SpriteAtlusId, SpriteIndexStart, SpriteIndexEnd, Framerate) \
	{\
		auto newAnim = CreateSpriteAnimation(spriteLib, SpriteAtlusID::SpriteAtlusId, SpriteIndexStart, SpriteIndexEnd, Framerate);\
		AddSpriteAnimation(newAnim);\
	}

		SPRITE_ANIMATION_LIST

#undef SPRITE_ANIMATION_LIST

		DEBUG_LOG("Animation Library loading complete");
}

SpriteAnimation AnimationLibrary::CreateSpriteAnimation(SpriteLibrary * spriteLib, SpriteAtlusID::Enum spriteAtlus, u8 spriteIndexStart, u8 spriteIndexEnd, u32 framerate)
{
	SpriteAnimation newAnim;
	const int maxFrameCount = spriteIndexEnd - spriteIndexStart;
	newAnim.keyFrames.Reserve(maxFrameCount);
		
	for (int i = 0; i < maxFrameCount; ++i)
	{
		Sprite* sprite = spriteLib->GetSprite(spriteAtlus, spriteIndexStart + i);
		SpriteAnimation::KeyFrame* keyframe = newAnim.keyFrames.AddNew();
		keyframe->sprite = sprite;
	}
			
	newAnim.frameRate = framerate;

	return newAnim;
}

void AnimationLibrary::AddSpriteAnimation(const SpriteAnimation & animation)
{
	m_spriteAnimations.Add(animation);
}

const SpriteAnimation * AnimationLibrary::GetSpriteAnimation(SpriteAnimationID::Enum animationId)
{
	return &m_spriteAnimations[animationId];
}
