#include "RpgInteraction.h"
#include "game/scripts/rulestates/GameRulestate.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/transformation/Transform.h"
#include "game/scripts/componentsystems/collision/Collider.h"
#include "game/scripts/componentsystems/movement/RpgMovement.h"
#include "engine/gameobject/GameObject.h"

void System::RpgInteraction::TryInteract(
	GameObject* interactorObject,
	GameRulestateParams & params
)
{
	auto* entityManager = params.engine->GetEntityRegistry();

	const Component::RpgInteractor & interactor = *interactorObject->GetComponent<Component::RpgInteractor>();
	const Component::Transform& interactorTransform = *interactorObject->GetComponent<Component::Transform>();
	const Component::Collider& interactorCollider = *interactorObject->GetComponent<Component::Collider>();
	const Component::RpgMovement& interactorMovement = *interactorObject->GetComponent<Component::RpgMovement>();

	auto currentVecFacing = interactorMovement.GetCurrentVecFacing();
	auto facingOffset = Vector2<tFixedPoint8>(currentVecFacing.x * interactor.facingProjectionOffset.x, currentVecFacing.y * interactor.facingProjectionOffset.y);

	// Check if the object is actually in front of the player by creating a new trigger box
	auto interactionCheckPosition = interactorTransform.GetPosition();
	interactionCheckPosition += facingOffset;

	Component::Collider interactorTrigger = interactorCollider;
	interactorTrigger.shape = interactor.interactionTrigger;

	// Ideally we'd check if the player index equals the input controller index, but we over have 1 player so screw it
	auto colliderView = entityManager->View<Component::RpgInteractable, Component::Collider, Component::Transform>();
	for (auto colliderEntity : colliderView)
	{
		const auto* otherCollider = entityManager->GetComponent<Component::Collider>(colliderEntity);
		const auto* otherTransform = entityManager->GetComponent<Component::Transform>(colliderEntity);

		System::Collision::Collision collision;
		if (System::Collision::DoesCollide(
			interactorTrigger,
			interactionCheckPosition,
			*otherCollider,
			otherTransform->GetPosition(), true, collision))
		{
			const auto* interactable = entityManager->GetComponent<Component::RpgInteractable>(colliderEntity);
			if (interactable->onInteracted)
				interactable->onInteracted(interactorObject, params);
		}
	}
}
