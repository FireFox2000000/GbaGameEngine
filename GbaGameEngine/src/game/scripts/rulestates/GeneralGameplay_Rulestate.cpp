#include "GeneralGameplay_Rulestate.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"

void GeneralGameplay_Rulestate::Enter(GameRulestateParams& params)
{
	DEBUG_LOG("Entered rulestate [GeneralGameplay_Rulestate]");
}

void GeneralGameplay_Rulestate::Update(GameRulestateParams& params)
{
	//params.stateMachine->ChangeState<Dialogue_Rulestate>(params);
	//return;
}
