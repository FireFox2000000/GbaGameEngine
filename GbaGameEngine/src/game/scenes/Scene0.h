#pragma once

#include "engine/scene/Scene.h"
#include "game/scripts/rulestates/GameRulestate.h"

class Scene0 : public Scene
{
	GameStateMachine m_rulestateMachine;
	std::unique_ptr<GameObject> background;
	std::unique_ptr<GameObject> player;
	std::unique_ptr<GameObject> testCollider;
public:
	Scene0(Engine* engine);
	~Scene0();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
