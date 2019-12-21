#pragma once
#include "engine/animation/SpriteAnimation.h"
#include "engine/asset/libraries/SpriteLibrary.h"

#define SPRITE_ANIMATION_LIST \
	SPRITE_ANIM_ENTRY(Shantae_Idle, Shantae_Idle, 0, 12, 12)

namespace SpriteAnimationID
{
#define SPRITE_ANIM_ENTRY(AnimName, SpriteAtlusId, SpriteIndexStart, SpriteIndexEnd, Framerate) AnimName,
	enum Enum
	{
		SPRITE_ANIMATION_LIST
		Count
	};
#undef SPRITE_ANIM_ENTRY
}

class AnimationLibrary
{
	FixedList<SpriteAnimation, SpriteAnimationID::Count> m_spriteAnimations;

	SpriteAnimation CreateSpriteAnimation(SpriteLibrary * spriteLib, SpriteAtlusID::Enum spriteAtlus, u8 spriteIndexStart, u8 spriteIndexEnd, u32 framerate);
	void AddSpriteAnimation(const SpriteAnimation& animation);

public:
	AnimationLibrary(SpriteLibrary * spriteLib);

	const SpriteAnimation* GetSpriteAnimation(SpriteAnimationID::Enum animationId);
};