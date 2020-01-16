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

protected:
	void Enter(GameRulestateParams& params) override;
	void Update(GameRulestateParams& params) override;

public:
	Dialogue_Rulestate(const std::string& script, SharedPtr<GameRulestate> finishedState);
};
