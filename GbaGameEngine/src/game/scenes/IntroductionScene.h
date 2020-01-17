#pragma once

#include "engine/scene/Scene.h"
#include "game/scripts/rulestates/GameRulestate.h"

class IntroductionScene : public Scene
{
	GameStateMachine m_rulestateMachine;

public:
	IntroductionScene(Engine* engine);
	~IntroductionScene();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
