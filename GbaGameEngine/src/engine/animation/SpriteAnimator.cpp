#include "SpriteAnimator.h"
#include "engine/engine/engine.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/time/Time.h"

void Component::SpriteAnimator::SetAnimation(const SpriteAnimation* animation)
{
	if (currentAnimation != animation)
	{
		currentAnimation = animation;

		timeToNextFrameMicroSeconds = 0;
		frameDtMicroseconds = SECONDS_TO_MICROSECONDS(1.0f / animation->frameRate);
		currentFrameIndex = 0;
	}
}

void System::SpriteAnimator::Update(Engine* engine)
{
	auto* entityManager = engine->GetEntityRegistry();

	const Time* time = engine->GetComponent<Time>();
	const u32 dtMicroSeconds = time->GetDtMicroSeconds();

	entityManager->InvokeEach<Component::SpriteAnimator, Component::SpriteRenderer>([&dtMicroSeconds](Component::SpriteAnimator& animator, Component::SpriteRenderer& spriteRenderer)
		{
			if (animator.FrameCount() <= 0)
				return;

			animator.timeToNextFrameMicroSeconds += dtMicroSeconds;

			u8 previousFrameIndex = animator.currentFrameIndex;

			// Advance current frame time and increment current frame index as needed
			while (animator.timeToNextFrameMicroSeconds > (s32)animator.frameDtMicroseconds)
			{
				animator.timeToNextFrameMicroSeconds -= animator.frameDtMicroseconds;
				++animator.currentFrameIndex;
			}

			// Wrap into a valid index
			while (animator.currentFrameIndex >= animator.FrameCount())
			{
				animator.currentFrameIndex -= animator.FrameCount();
			}

			if (previousFrameIndex != animator.currentFrameIndex)
			{
				Sprite* sprite = animator.currentAnimation->keyFrames[animator.currentFrameIndex].sprite;
				spriteRenderer.SetSprite(sprite);
			}
		});
}
