#include "AnimationTest.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/graphicalassets/sprite/SpriteLibrary.h"
#include "engine/math/Math.h"
#include "engine/render/SpriteRenderer.h"

Component::AnimationTest::AnimationTest(Engine* engine)
	: m_totalFrames(12)
	, m_startTimeMilliseconds(0)
{
	SetFrameRate(12);

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
}

Component::AnimationTest::~AnimationTest()
{
}

void Component::AnimationTest::SetFrameRate(u32 fps)
{
	m_frameRateMs = SECONDS_TO_MILLISECONDS(m_totalFrames * m_totalFrames / fps);
}

u32 Component::AnimationTest::GetFrameRate()
{
	return MILLISECONDS_TO_SECONDS(m_frameRateMs);
}


void System::AnimationTest::Update(Engine* engine)
{
	auto* entityManager = engine->GetEntityRegistry();

	Time* time = engine->GetTime();
	entityManager->InvokeEach<Component::AnimationTest, Component::SpriteRenderer>([&time](Component::AnimationTest& animation, Component::SpriteRenderer& spriteRenderer)
		{
			u32 msSinceAnimStart = time->GetMilliseconds() - animation.m_startTimeMilliseconds;
			u32 animCycleDuration = animation.m_frameRateMs / animation.m_totalFrames;

			u32 cycleRemainder = msSinceAnimStart % animCycleDuration;

			// Todo, search for frame based on keyframes

			u32 frame = cycleRemainder * animation.m_totalFrames / animCycleDuration;

			Sprite* sprite = animation.m_keyFrames[frame].sprite;
			spriteRenderer.SetSprite(sprite);
		});
}
