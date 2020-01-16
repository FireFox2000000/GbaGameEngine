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
		std::string script;
		script += "Testing testing";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		script += "1, 2, 3 testing";

		SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>(script, std::make_shared<GeneralGameplay_Rulestate>());
		params.stateMachine->ChangeState(dialogueRulestate, params);
	}

}

