#include "RpgMovement.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/time/Time.h"

Component::RpgMovement::RpgMovement()
	: movementAnimations(nullptr)
{
}

void System::RpgMovement::Update(Engine* engine)
{
	const Time* time = engine->GetComponent<Time>();

	auto dt = tFixedPoint24(time->GetDtSecondsf());

	auto* entityManager = engine->GetEntityRegistry();

	entityManager->InvokeEach<Component::Transform, Component::RpgMovement>(
		[&dt]
	(Component::Transform& transform, Component::RpgMovement& rpgMovementComponent)
		{
			auto position = transform.GetLocalPosition();
			tFixedPoint8 posDt = (tFixedPoint8)(rpgMovementComponent.speed * dt);

			switch (rpgMovementComponent.currentDirection)
			{
			case Component::RpgMovement::Up:
			{
				position.y += posDt;
				break;
			}
			case Component::RpgMovement::Down:
			{
				position.y -= posDt;
				break;
			}
			case Component::RpgMovement::Left:
			{
				position.x -= posDt;
				break;
			}
			case Component::RpgMovement::Right:
			{
				position.x += posDt;
				break;
			}
			default:
				break;
			}

			transform.SetLocalPosition(position);

			// Todo, set animations, check collisions

			const SpriteAnimation* animation = rpgMovementComponent.movementAnimations[rpgMovementComponent.currentDirection];
			if (animation)
			{

			}
		});
}

////////////////////////////////////////////////////////////////
#include "game/scripts/componentsystems/PlayerComponent.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "game/config/InputActions.h"

void System::RpgMovement::UpdatePlayerMovement(Engine * engine)
{
	auto* entityManager = engine->GetEntityRegistry();

	entityManager->InvokeEach<Component::RpgMovement, Component::Player>(
		[]
	(Component::RpgMovement& rpgMovementComponent, Component::Player& player)
		{
			using namespace GBA;

			Component::RpgMovement::Direction dir = Component::RpgMovement::None;

			if (Input::GetKey(InputActions::MoveLeft))
			{
				dir = Component::RpgMovement::Left;
			}

			if (Input::GetKey(InputActions::MoveRight))
			{
				dir = Component::RpgMovement::Right;
			}

			if (Input::GetKey(InputActions::MoveUp))
			{
				dir = Component::RpgMovement::Up;
			}

			if (Input::GetKey(InputActions::MoveDown))
			{
				dir = Component::RpgMovement::Down;
			}

			rpgMovementComponent.currentDirection = dir;
		});
}
