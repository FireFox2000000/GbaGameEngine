#pragma once

#include "engine/scene/Scene.h"
#include "engine/animation/SpriteAnimator.h"

class Scene0 : public Scene
{
	GameObject* playerObject;
	GameObject* textObject;

public:
	Scene0(Engine* engine);
	~Scene0();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
