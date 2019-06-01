#include "AnimationTest.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/graphicalassets/sprite/SpriteLibrary.h"
#include "engine/math/Math.h"
#include "engine/render/SpriteRenderer.h"

Component::AnimationTest::AnimationTest(Engine* engine)
{
	const int maxFrameCount = 12;
	m_keyFrames.Reserve(maxFrameCount);

	SpriteLibrary* spriteLibrary = engine->GetSpriteManager()->GetSpriteLibrary();

	for (int i = 0; i < maxFrameCount; ++i)
	{
		Sprite* sprite = spriteLibrary->GetSprite(SpriteAtlusID::Shantae_Idle, i);
		KeyFrame keyframe;
		keyframe.sprite = sprite;

		m_keyFrames.Add(keyframe);
	}

	SetFrameRate(12);
}

Component::AnimationTest::~AnimationTest()
{
}

void Component::AnimationTest::SetFrameRate(u32 fps)
{
	m_frameDtMicroseconds = SECONDS_TO_MICROSECONDS(1.0f / fps);
}

void System::AnimationTest::Update(Engine* engine)
{
	auto* entityManager = engine->GetEntityRegistry();

	Time* time = engine->GetTime();
	entityManager->InvokeEach<Component::AnimationTest, Component::SpriteRenderer>([&time](Component::AnimationTest& animation, Component::SpriteRenderer& spriteRenderer)
		{
			u32 dtMicroSeconds = time->GetDtMicroSeconds();
			animation.m_timeToNextFrameMicroSeconds += dtMicroSeconds;

			// Advance current frame time and increment current frame index as needed
			while (animation.m_timeToNextFrameMicroSeconds > (s32)animation.m_frameDtMicroseconds)
			{
				animation.m_timeToNextFrameMicroSeconds -= animation.m_frameDtMicroseconds;
				++animation.m_currentFrameIndex;
			}

			// Wrap into a valid index
			while (animation.m_currentFrameIndex >= animation.FrameCount())
			{
				animation.m_currentFrameIndex -= animation.FrameCount();
			}

			Sprite* sprite = animation.m_keyFrames[animation.m_currentFrameIndex].sprite;
			spriteRenderer.SetSprite(sprite);
		});
}
