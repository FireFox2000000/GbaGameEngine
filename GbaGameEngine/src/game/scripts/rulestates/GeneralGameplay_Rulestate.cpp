#include "GeneralGameplay_Rulestate.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "game/scripts/componentsystems/movement/RpgMovement.h"
#include "game/config/InputActions.h"

void GeneralGameplay_Rulestate::Enter(GameRulestateParams& params)
{
	DEBUG_LOG("Entered rulestate [GeneralGameplay_Rulestate]");
}

void GeneralGameplay_Rulestate::Update(GameRulestateParams& params)
{
	using namespace GBA;

	if (Input::GetKeyDown(InputActions::Interact))
	{
		std::string script;
		script += "Testing \ntesting";
		script += Dialogue_Rulestate::c_dialogueBoxStepFlag;
		script += "1, 2, 3 testing";

		SharedPtr<GameRulestate> dialogueRulestate = std::make_shared<Dialogue_Rulestate>(script, std::make_shared<GeneralGameplay_Rulestate>());
		params.stateMachine->ChangeState(dialogueRulestate, params);
	}

	System::RpgMovement::UpdatePlayerMovement(params.engine);
	System::RpgMovement::Update(params.engine);
}

