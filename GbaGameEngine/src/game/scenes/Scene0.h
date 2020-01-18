#pragma once

#include "engine/scene/Scene.h"
#include "game/scripts/rulestates/GameRulestate.h"

class Scene0 : public Scene
{
	GameStateMachine m_rulestateMachine;
	std::unique_ptr<GameObject> background;
	std::unique_ptr<GameObject> player;
	FixedList<GameObject, 2> propObjects;

	void SetupSceneProps(Engine* engine);

public:
	Scene0(Engine* engine);
	~Scene0();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
