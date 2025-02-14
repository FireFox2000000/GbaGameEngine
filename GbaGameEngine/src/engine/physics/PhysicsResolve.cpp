#include "PhysicsResolve.h"
#include "PhysicsConfig.h"
#include "engine/math/Math.h"
#include "engine/math/VectorMath.h"
#include "engine/engine/engine.h"
#include "engine/transform/Transform2.h"
#include "engine/physics/Rigidbody.h"
#include "engine/physics/Collider.h"
#include "engine/physics/Collision.h"
#include "engine/physics/CollisionFunctions.h"

// Accumulates FPi24 clamped to [-127, 127] range without wrapping around
Vector2<FPi16> SafeAddVelocityClamped(const Vector2<FPi16>& velocity, const Vector2<FPi16>& delta)
{
	Vector2i velWholeNum = Vector2i(velocity);
	Vector2<FPi16> velDecimal = Vector2<FPi16>(
		velocity.x - FPi16(velWholeNum.x),
		velocity.y - FPi16(velWholeNum.y)
		);

	velDecimal += delta;

	// Remove whole numbers from velDecimal that may have just accumulated
	velWholeNum.x += static_cast<int>(velDecimal.x);
	velWholeNum.y += static_cast<int>(velDecimal.y);

	velDecimal.x -= FPi16(static_cast<int>(velDecimal.x));
	velDecimal.y -= FPi16(static_cast<int>(velDecimal.y));

	velWholeNum.x = Math::Clamp(velWholeNum.x, -127, 127);
	velWholeNum.y = Math::Clamp(velWholeNum.y, -127, 127);

	return Vector2<FPi16>(velDecimal.x + FPi16(velWholeNum.x), velDecimal.y + FPi16(velWholeNum.y));
}

void System::Physics::UpdateTransforms()
{
	auto* entityManager = Engine::GetInstance().GetEntityRegistry();

	entityManager->InvokeEach<Transform2, Component::Rigidbody>(
		[]
	(Transform2& transform, Component::Rigidbody& rigidbody)
		{			
			// Add any impulses

			// Add gravitational force
			auto deltaVel = rigidbody.gravity * PhysicsConfig::c_fixedUpdateRate;

			rigidbody.velocity = SafeAddVelocityClamped(rigidbody.velocity, deltaVel);

			// Finally update the transform's position
			auto position = transform.GetPosition();
			position += static_cast<decltype(position)>(rigidbody.velocity * PhysicsConfig::c_fixedUpdateRate);

			// Hacky ground clamp for testing
			//if (position.y < -5)
			//{
			//	position.y = -5;
			//	rigidbody.velocity.y = 0;
			//}

			transform.SetPosition(position);
		});
}

void System::Physics::ResolveCollisions()
{
	auto* entityManager = Engine::GetInstance().GetEntityRegistry();

	entityManager->InvokeEach<Transform2, Component::Rigidbody, Component::Collider>(
		[&entityManager]
	(ECS::Entity entityA, Transform2& transformA, Component::Rigidbody& rigidbodyA, Component::Collider& colliderA)
		{
			// Collide against static objects, dynamic objects will be more... todo
			entityManager->InvokeEach<Transform2, Component::Collider>(
				[&]
			(ECS::Entity entityB, Transform2& transformB,  Component::Collider& colliderB)
				{
					if (entityA == entityB) return;
					if (!(colliderA.GetCollisionMask() & colliderB.GetCollisionMask())) return;

					Collision collision;
					Collision* collisionHandle = colliderB.GetIsTrigger() ? nullptr : &collision;

					// TODO, should do sweep collisions instead
					if (CollisionFunctions::HasCollision(transformA, colliderA, transformB, colliderB, collisionHandle))
					{
						collision.entityA = entityA;
						collision.entityB = entityB;

						if (collisionHandle)
						{
							// Push the rigidbody out of the penetrated collider
							auto position = transformA.GetPosition();
							position += static_cast<decltype(position)>(collision.normal * collision.penetrationDepth);
							transformA.SetPosition(position);

							auto velInv = rigidbodyA.velocity * -1;

							// Kill velocity against the normal if we're moving into the collider. 
							// Otherwise preserve momentumn if we're travelling in the same direction as the push.
							if (velInv.MagnitudeSqrd() > 0 && VectorMath::Dot(velInv, collision.normal) > 0)
							{
								auto normalProj = VectorMath::ProjectionNormalised(velInv, collision.normal);
								rigidbodyA.velocity += normalProj;
							}
						}

						// Notify collision
						colliderA.OnHit(collision);
						colliderB.OnHit(collision);
					}
				});
		});
}
