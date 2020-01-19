#include "Collider.h"

bool System::Collision::DoesCollide
	(const Component::Collider& colliderA
	, const Component::Transform::tPosition& positionA
	, const Component::Collider& colliderB
	, const Component::Transform::tPosition& positionB
	, bool touchEnabled
	, Collision& out_collision
	)
{
	if (!colliderA.mask.TestMask(colliderB.mask))
		return false;

	out_collision.isTrigger = colliderA.isTrigger || colliderB.isTrigger;

	if (!colliderA.shapeInverted && !colliderB.shapeInverted)
	{
		auto worldShapeA = colliderA.shape;
		worldShapeA.Translate(positionA);

		auto worldShapeB = colliderB.shape;
		worldShapeB.Translate(positionB);

		bool collides = touchEnabled ? worldShapeA.IntersectsOrTouches(worldShapeB) : worldShapeA.Intersects(worldShapeB);
		if (collides)
		{
			if (worldShapeA.min.x < worldShapeB.min.x)	// A is left of B
			{
				out_collision.aToBProjection.x = worldShapeB.min.x - worldShapeA.max.x;
			}
			else
			{
				out_collision.aToBProjection.x = worldShapeB.max.x - worldShapeA.min.x;
			}

			if (worldShapeA.min.y < worldShapeB.min.y)	// A is below B
			{
				out_collision.aToBProjection.y = worldShapeB.min.y - worldShapeA.max.y;
			}
			else
			{
				out_collision.aToBProjection.y = worldShapeB.max.y - worldShapeA.min.y;
			}
		}

		return collides;
	}

	if (colliderA.shapeInverted != colliderB.shapeInverted)
	{
		auto worldShapeA = colliderA.shape;
		worldShapeA.Translate(positionA);

		auto worldShapeB = colliderB.shape;
		worldShapeB.Translate(positionB);

		const auto& invertedCol = colliderA.shapeInverted ? worldShapeA : worldShapeB;
		const auto& regularCol = !colliderA.shapeInverted ? worldShapeA : worldShapeB;

		if (worldShapeA.max.x > worldShapeB.max.x)	// A is peeking right of B
		{
			out_collision.aToBProjection.x = worldShapeB.max.x - worldShapeA.max.x;
		}
		else
		{
			out_collision.aToBProjection.x = worldShapeB.min.x - worldShapeA.min.x;
		}

		if (worldShapeA.max.y > worldShapeB.max.y)	// A is peaking above B
		{
			out_collision.aToBProjection.y = worldShapeB.max.y - worldShapeA.max.y;
		}
		else
		{
			out_collision.aToBProjection.y = worldShapeB.min.y - worldShapeA.min.y;
		}

		return !(invertedCol.Contains(regularCol.min) && invertedCol.Contains(regularCol.max));
	}

	return false;
}
