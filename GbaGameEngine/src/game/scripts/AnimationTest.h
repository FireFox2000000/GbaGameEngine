#ifndef PRAGMA_ONCE_GAME_SCRIPTS_ANIMATIONTEST_H
#define PRAGMA_ONCE_GAME_SCRIPTS_ANIMATIONTEST_H

#include "engine/base/core/stl/List.h"
#include "engine/component/MonoBehaviour.h"

class SpriteRenderer;
class Sprite;

class AnimationTest : public MonoBehaviour
{
	SpriteRenderer* m_spriteRenderer;
	struct KeyFrame
	{
		Sprite* sprite;
	};
	
	typedef List<KeyFrame> KeyFrames;
	
	u32 m_frameRate;
	u32 m_totalFrames;
	KeyFrames m_keyFrames;
	double m_startTime;

public:
	AnimationTest(GameObject* gameObject);
	~AnimationTest();

	void Init(Engine* engine);
	void Update(Engine* engine);
};

#endif
