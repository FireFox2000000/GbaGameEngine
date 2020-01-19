#pragma once

#include "engine/scene/Scene.h"
#include "game/scripts/rulestates/GameRulestate.h"

class IntroductionScene : public Scene
{
	GameStateMachine m_rulestateMachine;
	std::unique_ptr<GameObject> m_player;

public:
	IntroductionScene(Engine* engine);
	~IntroductionScene();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
