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
		keyframe.frameIndex = i;

		m_keyFrames.Add(keyframe);
	}
}

void AnimationTest::Update(Engine* engine)
{
	Time* time = engine->GetTime();
	double timeSinceAnimationStart = time->GetTime() - m_startTime;
	double animCycleDuration = 1.0 / m_frameRate * m_totalFrames;

	double cycles = timeSinceAnimationStart / animCycleDuration;
	double cycleRemainder = cycles - int(cycles);

	// Todo, search for frame based on keyframes

	u32 frame = u32(cycleRemainder * m_totalFrames);

	Sprite* sprite = m_keyFrames[frame].sprite;
	m_spriteRenderer->SetSprite(sprite);
}
