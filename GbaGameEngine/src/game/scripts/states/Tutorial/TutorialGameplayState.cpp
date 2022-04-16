#include "TutorialGameplayState.h"
#include "engine/gameobject/transformation/Transform.h"

void TutorialGameplayState::Enter(TutorialStateParams* params)
{
	DEBUG_LOG("Entered TutorialGameplayState");

	// Show relevent gameobjects
	// Note, move this to a transition state so we can fade this in properly
	{
		auto* transform = params->sagumeGameObject->EditComponent<Component::Transform>();
		transform->SetPosition(0, 0);
	}
}

void TutorialGameplayState::Update(TutorialStateParams* params)
{
}
