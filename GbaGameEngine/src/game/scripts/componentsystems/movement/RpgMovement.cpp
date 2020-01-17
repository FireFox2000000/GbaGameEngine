#include "RpgMovement.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/time/Time.h"
#include "game/scripts/componentsystems/collision/Collider.h"
#include "engine/animation/SpriteAnimator.h"

Component::RpgMovement::RpgMovement()
	: movementAnimations(nullptr)
{
}

void System::RpgMovement::Update(Engine* engine)
{
	const Time* time = engine->GetComponent<Time>();

	auto dt = tFixedPoint24(time->GetDtSecondsf());

	auto* entityManager = engine->GetEntityRegistry();

	auto view = entityManager->View<Component::Transform, Component::RpgMovement>();
	for (auto entity : view)
	{
		const Component::RpgMovement& rpgMovementComponent = *entityManager->EditComponent<Component::RpgMovement>(entity);
		Component::Transform& transform = *entityManager->EditComponent<Component::Transform>(entity);

		auto position = transform.GetLocalPosition();

		// Update position
		{	
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
		}

		// Todo, set animations, check collisions

		if (Component::SpriteAnimator* animator = entityManager->EditComponent<Component::SpriteAnimator>(entity))
		{
			const SpriteAnimation* animation = rpgMovementComponent.movementAnimations[rpgMovementComponent.currentDirection];
			if (animation)
			{
			}
		}

		// Hack in place of actual physics system, don't actually use this with more than 1 rpgmovement user as this is brute force and really inefficient
		if (const Component::Collider* collider = entityManager->GetComponent<Component::Collider>(entity))
		{
			auto colliderView = entityManager->View<Component::Collider, Component::Transform>();
			for (auto colliderEntity : colliderView)
			{
				if (entity == colliderEntity)
					continue;

				System::Collision::Collision collision;
				if (System::Collision::DoesCollide(
					*collider,
					position,
					*entityManager->GetComponent<Component::Collider>(colliderEntity),
					entityManager->GetComponent<Component::Transform>(colliderEntity)->GetPosition(), collision))
				{
					DEBUG_LOGFORMAT("Collision projection (%f, %f)", collision.aToBProjection.x.ToFloat(), collision.aToBProjection.y.ToFloat());
					switch (rpgMovementComponent.currentDirection)
					{
					case Component::RpgMovement::Up:
					case Component::RpgMovement::Down:
					{
						position.y += collision.aToBProjection.y;
						break;
					}
					case Component::RpgMovement::Left:
					case Component::RpgMovement::Right:
					default:
					{
						position.x += collision.aToBProjection.x;
						break;
					}
					}

					break;
				}
			}


			transform.SetLocalPosition(position);
		}
	}
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
