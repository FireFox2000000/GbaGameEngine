#include "AnimationTest.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/graphicalassets/sprite/SpriteLibrary.h"
#include "engine/render/SpriteRenderer.h"

AnimationTest::AnimationTest(GameObject* gameObject)
	: MonoBehaviour(gameObject)
	, m_frameRate(12)
	, m_totalFrames(12)
	, m_startTime(0)
{
	m_spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
}


AnimationTest::~AnimationTest()
{
}

void AnimationTest::Init(Engine * engine)
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
}

void AnimationTest::Update(Engine* engine)
{
	Time* time = engine->GetTime();
	float timeSinceAnimationStart = time->GetTime() - m_startTime;
	float animCycleDuration = m_totalFrames / (float)m_frameRate;

	float cycles = timeSinceAnimationStart / animCycleDuration;
	float cycleRemainder = cycles - int(cycles);

	// Todo, search for frame based on keyframes

	u32 frame = u32(cycleRemainder * m_totalFrames);

	Sprite* sprite = m_keyFrames[frame].sprite;
	m_spriteRenderer->SetSprite(sprite);
}
