#include "AnimationTest.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/graphicalassets/sprite/SpriteLibrary.h"
#include "engine/math/Math.h"
#include "engine/render/SpriteRenderer.h"

AnimationTest::AnimationTest(GameObject* gameObject)
	: MonoBehaviour(gameObject)
	, m_totalFrames(12)
	, m_startTimeMilliseconds(0)
{
	SetFrameRate(12);
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
	u32 msSinceAnimStart = time->GetMilliseconds() - m_startTimeMilliseconds;
	u32 animCycleDuration = m_frameRateMs / m_totalFrames;

	u32 cycleRemainder = msSinceAnimStart % animCycleDuration;

	// Todo, search for frame based on keyframes

	u32 frame = cycleRemainder * m_totalFrames / animCycleDuration;

	Sprite* sprite = m_keyFrames[frame].sprite;
	m_spriteRenderer->SetSprite(sprite);
}

void AnimationTest::SetFrameRate(u32 fps)
{
	m_frameRateMs = SECONDS_TO_MILLISECONDS(m_totalFrames * m_totalFrames / fps);
}

u32 AnimationTest::GetFrameRate()
{
	return MILLISECONDS_TO_SECONDS(m_frameRateMs);
}
