#include "GeneralGameplay_Rulestate.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"
#include "engine/gba/registers/input/GBAInput.h"

void GeneralGameplay_Rulestate::Enter(GameRulestateParams& params)
{
	DEBUG_LOG("Entered rulestate [GeneralGameplay_Rulestate]");
}

void GeneralGameplay_Rulestate::Update(GameRulestateParams& params)
{
	using namespace GBA;

	if (Input::GetKeyDown(Buttons::A))
	{
		SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>("Testing testing \n1, 2, 3 testing", std::make_shared<GeneralGameplay_Rulestate>());
		params.stateMachine->ChangeState(dialogueRulestate, params);
	}

}

GeneralGameplay_Rulestate::~GeneralGameplay_Rulestate()
{
	DEBUG_LOG("Destroyed rulestate [GeneralGameplay_Rulestate]");
}
