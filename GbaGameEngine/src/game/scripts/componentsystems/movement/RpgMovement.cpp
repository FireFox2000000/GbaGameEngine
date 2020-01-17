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

Component::RpgMovement::Direction Component::RpgMovement::GetCurrentDirection() const
{
	return currentDirection;
}

Component::RpgMovement::Direction Component::RpgMovement::GetPreviousDirection() const
{
	return previousDirection;
}

void Component::RpgMovement::SetCurrentDirection(Direction dir)
{
	previousDirection = currentDirection;
	currentDirection = dir;
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
		auto scale = transform.GetLocalScale();
		auto currentDirection = rpgMovementComponent.GetCurrentDirection();

		// Update position
		{	
			tFixedPoint8 posDt = (tFixedPoint8)(rpgMovementComponent.speed * dt);

			switch (currentDirection)
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
				scale.x = Math::Abs(scale.x);
				break;
			}
			case Component::RpgMovement::Right:
			{
				position.x += posDt;
				scale.x = Math::Abs(scale.x) * tFixedPoint8(-1);
				break;
			}
			default:
				break;
			}
		}

		// Set animations, check collisions
		if (Component::SpriteAnimator* animator = entityManager->EditComponent<Component::SpriteAnimator>(entity))
		{
			bool idleAnim = currentDirection == Component::RpgMovement::Direction::None;
			const Component::RpgMovement::tAnimationContainer& animations = idleAnim ? rpgMovementComponent.idleAnimations : rpgMovementComponent.movementAnimations;
			const SpriteAnimation* animation = animations[idleAnim ? currentDirection : rpgMovementComponent.GetPreviousDirection()];
			if (animation)
			{
				animator->SetAnimation(animation);
			}
		}

		// Hack in place of actual physics system, don't actually use this with more than 1 rpgmovement user as this is brute force and really inefficient
		if (const Component::Collider* collider = entityManager->GetComponent<Component::Collider>(entity))
		{
			auto colliderView = entityManager->View<Component::Collider, Component::Transform>();
			for (auto colliderEntity : colliderView)
			{
				const auto* otherCollider = entityManager->GetComponent<Component::Collider>(colliderEntity);
				if (entity == colliderEntity || collider->isTrigger || otherCollider->isTrigger)
					continue;

				System::Collision::Collision collision;
				if (System::Collision::DoesCollide(
					*collider,
					position,
					*otherCollider,
					entityManager->GetComponent<Component::Transform>(colliderEntity)->GetPosition(), collision))
				{
					switch (currentDirection)
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

			if (rpgMovementComponent.enableHorizontalDirSpriteFlip)
				transform.SetLocalScale(scale);
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

			rpgMovementComponent.SetCurrentDirection(dir);
		});
}
