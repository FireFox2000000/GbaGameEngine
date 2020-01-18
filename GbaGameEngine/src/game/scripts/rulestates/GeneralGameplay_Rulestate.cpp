#include "GeneralGameplay_Rulestate.h"
#include "game/scripts/rulestates/Dialogue_Rulestate.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "game/config/InputActions.h"
#include "engine/engine/engine.h"

#include "game/scripts/componentsystems/movement/RpgMovement.h"
#include "game/scripts/componentsystems/interaction/RpgInteraction.h"

void GeneralGameplay_Rulestate::Enter(GameRulestateParams& params)
{
	DEBUG_LOG("Entered rulestate [GeneralGameplay_Rulestate]");
}

void GeneralGameplay_Rulestate::Update(GameRulestateParams& params)
{
	using namespace GBA;

	System::RpgMovement::UpdatePlayerMovement(params.engine);
	System::RpgMovement::Update(params.engine);

	if (Input::GetKeyDown(InputActions::Interact))
	{
		TryInteract(params);
	}
}

void GeneralGameplay_Rulestate::TryInteract(GameRulestateParams& params)
{
	System::RpgInteraction::TryInteract(
		params.playerObject,
		params);
}

