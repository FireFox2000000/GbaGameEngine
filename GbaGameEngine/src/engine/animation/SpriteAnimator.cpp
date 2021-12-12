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
		frameDtMicroseconds = currentAnimation ? SECONDS_TO_MICROSECONDS(1.0f / currentAnimation->frameRate) : 0;
		currentFrameIndex = -1;  // New animation
	}
}

void System::SpriteAnimator::Update(Engine* engine)
{
	auto* entityManager = engine->GetEntityRegistry();

	const Time* time = engine->GetComponent<Time>();
	const u32 dtMicroSeconds = time->GetDtTimeValue().TotalMicroseconds();

	entityManager->InvokeEach<Component::SpriteAnimator, Component::SpriteRenderer>([&dtMicroSeconds](Component::SpriteAnimator& animator, Component::SpriteRenderer& spriteRenderer)
		{
			if (!animator.currentAnimation || animator.FrameCount() <= 0)
				return;

			// A new animation was set
			if (animator.currentFrameIndex < 0)
			{
				animator.currentFrameIndex = 0;

				Sprite* sprite = animator.currentAnimation->keyFrames[animator.currentFrameIndex].sprite;
				spriteRenderer.SetSprite(sprite);

				return;
			}

			animator.timeToNextFrameMicroSeconds += dtMicroSeconds;

			u8 previousFrameIndex = animator.currentFrameIndex;

			// Advance current frame time and increment current frame index as needed
			while (animator.timeToNextFrameMicroSeconds > (s32)animator.frameDtMicroseconds)
			{
				animator.timeToNextFrameMicroSeconds -= animator.frameDtMicroseconds;
				++animator.currentFrameIndex;
			}

			// Wrap into a valid index
			while (animator.currentFrameIndex >= (int)animator.FrameCount())
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
