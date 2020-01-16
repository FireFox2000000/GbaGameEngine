#include "GeneralGameplay_Rulestate.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"

void GeneralGameplay_Rulestate::Enter(GameRulestateParams& params)
{
	DEBUG_LOG("Entered rulestate [GeneralGameplay_Rulestate]");
}

void GeneralGameplay_Rulestate::Update(GameRulestateParams& params)
{
	params.stateMachine->ChangeState(std::make_shared<Dialogue_Rulestate>(), params);
}

GeneralGameplay_Rulestate::~GeneralGameplay_Rulestate()
{
	DEBUG_LOG("Destroyed rulestate [GeneralGameplay_Rulestate]");
}
