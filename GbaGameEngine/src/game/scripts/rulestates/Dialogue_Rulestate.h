#pragma once

#include "GameRulestate.h"

class Dialogue_Rulestate : public GameRulestate
{
protected:
	void Enter(GameRulestateParams& params) override;
	void Update(GameRulestateParams& params) override;
};
