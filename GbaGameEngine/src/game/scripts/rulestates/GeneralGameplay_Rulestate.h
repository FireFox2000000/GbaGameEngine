#pragma once

#include "GameRulestate.h"

class GeneralGameplay_Rulestate : public GameRulestate
{
protected:
	void Enter(GameRulestateParams& params) override;
	void Update(GameRulestateParams& params) override;

public:
};
