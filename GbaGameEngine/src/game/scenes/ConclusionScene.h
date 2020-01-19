#pragma once

#include "engine/scene/Scene.h"
#include "game/scripts/rulestates/GameRulestate.h"

class ConclusionScene : public Scene
{
	GameStateMachine m_rulestateMachine;
	std::unique_ptr<GameObject> m_player;
	std::unique_ptr<GameObject> m_npcFairy;

	int m_dialogueIndex = 0;
	void NextCutsceneCommand(int index, GameRulestateParams& params);
public:
	ConclusionScene(Engine* engine);
	~ConclusionScene();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
