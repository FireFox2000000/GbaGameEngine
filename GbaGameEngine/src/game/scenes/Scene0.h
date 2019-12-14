#pragma once

#include "engine/scene/Scene.h"
#include "engine/animation/SpriteAnimator.h"

class Scene0 : public Scene
{
public:
	Scene0(Engine* engine);
	~Scene0();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;

	SpriteAnimation m_idleAnim;
};
