#pragma once

#include "GameRulestate.h"
#include "engine/base/core/stl/List.h"
#include <string>
#include "engine/gameobject/GameObject.h"

class Dialogue_Rulestate : public GameRulestate
{
	std::string m_script;
	SharedPtr<GameRulestate> m_finishedState;

	std::unique_ptr<GameObject> m_dialogueObject;

	int m_currentTextIndex = 0;

	bool AdvanceText();

protected:
	void Enter(GameRulestateParams& params) override;
	void Update(GameRulestateParams& params) override;

public:
	const static char c_dialogueBoxStepFlag = '\n';

	Dialogue_Rulestate(const std::string& script, SharedPtr<GameRulestate> finishedState);
};
