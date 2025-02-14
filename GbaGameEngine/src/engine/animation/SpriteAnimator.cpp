#include "SpriteAnimator.h"
#include "engine/engine/engine.h"
#include "engine/graphics/sprite/SpriteRenderer.h"
#include "engine/time/Time.h"

void SpriteAnimator::SetAnimation(const SpriteAnimation* animation)
{
	if (m_currentAnimation != animation)
	{
		m_currentAnimation = animation;

		m_timeToNextFrameMicroSeconds = 0;
		m_frameDtMicroseconds = m_currentAnimation ? SECONDS_TO_MICROSECONDS(1.0f / m_currentAnimation->frameRate) : 0;
		m_currentFrameIndex = -1;  // New animation
	}
}

void System::UpdateSpriteAnimators()
{
	auto* entityManager = Engine::GetInstance().GetEntityRegistry();

	const Time* time = Engine::GetInstance().GetComponent<Time>();
	const u32 dtMicroSeconds = time->GetDtTimeValue().TotalMicroseconds();

	entityManager->InvokeEach<SpriteAnimator, SpriteRenderer>([&dtMicroSeconds](ECS::Entity entity, SpriteAnimator& animator, SpriteRenderer& spriteRenderer)
		{
			if (!animator.m_currentAnimation || animator.FrameCount() <= 0)
				return;

			// A new animation was set
			if (animator.m_currentFrameIndex < 0)
			{
				animator.m_currentFrameIndex = 0;

				Sprite* sprite = animator.m_currentAnimation->keyFrames[animator.m_currentFrameIndex].sprite;
				spriteRenderer.SetSprite(sprite);

				if (animator.m_currentAnimation->onNewFrameHandler)
				{
					animator.m_currentAnimation->onNewFrameHandler(animator.m_currentFrameIndex, entity, &animator);
				}

				return;
			}

			animator.m_timeToNextFrameMicroSeconds += dtMicroSeconds;

			u8 previousFrameIndex = animator.m_currentFrameIndex;

			// Advance current frame time and increment current frame index as needed
			while (animator.m_timeToNextFrameMicroSeconds > static_cast<s32>(animator.m_frameDtMicroseconds))
			{
				animator.m_timeToNextFrameMicroSeconds -= animator.m_frameDtMicroseconds;
				++animator.m_currentFrameIndex;
			}

			// Wrap into a valid index
			DEBUG_ASSERTMSGFORMAT(static_cast<int>(animator.FrameCount()) >= 0, "Invalid frame count for animation %d", static_cast<int>(animator.FrameCount()));
			while (animator.m_currentFrameIndex >= static_cast<int>(animator.FrameCount()))
			{
				animator.m_currentFrameIndex -= animator.FrameCount();
			}

			if (previousFrameIndex != animator.m_currentFrameIndex)
			{
				Sprite* sprite = animator.m_currentAnimation->keyFrames[animator.m_currentFrameIndex].sprite;
				spriteRenderer.SetSprite(sprite);

				if (animator.m_currentAnimation->onNewFrameHandler)
				{
					animator.m_currentAnimation->onNewFrameHandler(animator.m_currentFrameIndex, entity, &animator);
				}
			}
		});
}
