#include "CollisionFunctions.h"
#include "Collider.h"
#include "Collision.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/math/VectorMath.h"

// Helpful resources for future self:
// https://noonat.github.io/intersect/
// https://www.gamasutra.com/view/feature/3383/simple_intersection_tests_for_games.php?print=1

inline AxisAlignedBoundingBox2 AdjustAABBByTransform(const Component::Transform& transformA, AxisAlignedBoundingBox2 aabb)
{
	// TODO, maybe AABB should only adjust by position? And use OBB instead when doing these kinds of transforms? Depends on the game I guess. 
	// AABB with scale transformations applies would be cheaper than full OBB checks

	auto scale = transformA.GetScale();

	Vector2<tFixedPoint24> min = aabb.min;
	Vector2<tFixedPoint24> max = aabb.max;

	min.x *= scale.x;
	min.y *= scale.y;
	max.x *= scale.x;
	max.y *= scale.y;

	Vector2<tFixedPoint8> newMinA(tFixedPoint8(MIN(min.x, max.x)), tFixedPoint8(MIN(min.y, max.y)));
	Vector2<tFixedPoint8> newMaxA(tFixedPoint8(MAX(min.x, max.x)), tFixedPoint8(MAX(min.y, max.y)));

	aabb = AxisAlignedBoundingBox2(newMinA, newMaxA);

	aabb.Translate(transformA.GetPosition());

	return aabb;
}

/********************************************************************************************************/

bool HasCollisionAABBvsAABB(
	const Component::Transform& transformA
	, AxisAlignedBoundingBox2 colA
	, const Component::Transform& transformB
	, AxisAlignedBoundingBox2 colB
	, Collision* out_collisionMaybe)
{
	colA = AdjustAABBByTransform(transformA, colA);
	colB = AdjustAABBByTransform(transformB, colB);

	bool result = colA.IntersectsOrTouches(colB);

	if (result && out_collisionMaybe)
	{
		Vector2<tFixedPoint8> halfExtentsA = (colA.max - colA.min) * tFixedPoint8(0.5f);
		Vector2<tFixedPoint8> aabbCenterA = colA.min + halfExtentsA;

		Vector2<tFixedPoint8> halfExtentsB = (colB.max - colB.min) * tFixedPoint8(0.5f);
		Vector2<tFixedPoint8> aabbCenterB = colB.min + halfExtentsB;

		tFixedPoint8 dx = aabbCenterB.x - aabbCenterA.x;
		tFixedPoint8 px = (halfExtentsB.x + halfExtentsA.x) - Math::Abs(dx);

		tFixedPoint8 dy = aabbCenterB.y - aabbCenterA.y;
		tFixedPoint8 py = (halfExtentsB.y + halfExtentsA.y) - Math::Abs(dy);

		if (px < py) 
		{
			tFixedPoint8 sx = Math::Sign(dx);
			out_collisionMaybe->penetrationDepth = Math::Abs(tFixedPoint24(px));
			out_collisionMaybe->normal = Vector2<tFixedPoint24>(tFixedPoint24(sx) * -1, 0);
		}
		else 
		{
			tFixedPoint8 sy = Math::Sign(dy);
			out_collisionMaybe->penetrationDepth = Math::Abs(tFixedPoint24(py));
			out_collisionMaybe->normal = Vector2<tFixedPoint24>(0, tFixedPoint24(sy) * -1);
		}
	}

	return result;
}

bool HasCollisionCirclevsCircle(
	const Vector2<tFixedPoint8>& positionA
	, Circle colA
	, const Vector2<tFixedPoint8>& positionB
	, Circle colB
	, Collision* out_collisionMaybe)
{
	tFixedPoint8 lengthSqrd = VectorMath::LengthSqrd(positionA, positionB);
	tFixedPoint8 rad = colA.radius + colB.radius;
	tFixedPoint8 radSqrd = rad * rad;

	bool result = radSqrd >= lengthSqrd;

	if (result && out_collisionMaybe)
	{
		Vector2<tFixedPoint8> direction = positionA - positionB;
		Vector2<tFixedPoint24> normal = VectorMath::Normalised(direction);

		out_collisionMaybe->normal = normal;
		out_collisionMaybe->penetrationDepth = tFixedPoint24(rad - tFixedPoint8(std::sqrt(lengthSqrd.ToFloat())));	// float heavy, but need accuracy or this doesn't work.
	}

	return result;
}

bool HasCollisionAABBvsCircle(
	const Component::Transform& transformA
	, AxisAlignedBoundingBox2 colA
	, const Vector2<tFixedPoint8>& positionB
	, Circle colB
	, Collision* out_collisionMaybe)
{
	// https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-Detection - AABB - Circle collision detection

	colA = AdjustAABBByTransform(transformA, colA);
	Vector2<tFixedPoint8> halfExtents = (colA.max - colA.min) * tFixedPoint8(0.5f);
	Vector2<tFixedPoint8> aabbCenter = colA.min + halfExtents;

	Vector2<tFixedPoint8> delta = positionB - aabbCenter;
	Vector2<tFixedPoint8> clamped(
		MAX(tFixedPoint8(-1) * halfExtents.x, MIN(halfExtents.x, delta.x)),
		MAX(tFixedPoint8(-1) * halfExtents.y, MIN(halfExtents.y, delta.y))
		);

	Vector2<tFixedPoint8> closestPoint = aabbCenter + clamped;

	delta = closestPoint - positionB;

	bool result = delta.MagnitudeSqrd() < (colB.radius * colB.radius);

	if (result && out_collisionMaybe)
	{
		Vector2<tFixedPoint8> direction = closestPoint - positionB;
		Vector2<tFixedPoint24> normal = VectorMath::Normalised(direction);

		out_collisionMaybe->normal = normal;
		out_collisionMaybe->penetrationDepth = tFixedPoint24(colB.radius - tFixedPoint8(std::sqrt(direction.MagnitudeSqrd().ToFloat())));	// float heavy, but need accuracy or this doesn't work.		
	}

	return result;
}

bool HasCollisionAABBvsAABB(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB
	, Collision* out_collisionMaybe)
{
	return HasCollisionAABBvsAABB(transformA, colA.GetAABB(), transformB, colB.GetAABB(), out_collisionMaybe);
}

/********************************************************************************************************/

bool HasCollisionCirclevsCircle(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB
	, Collision* out_collisionMaybe)
{
	return HasCollisionCirclevsCircle(transformA.GetPosition(), colA.GetCircle(), transformB.GetPosition(), colB.GetCircle(), out_collisionMaybe);
}

bool HasCollisionAABBvsCircle(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB
	, Collision* out_collisionMaybe)
{
	return HasCollisionAABBvsCircle(transformA, colA.GetAABB(), transformB.GetPosition(), colB.GetCircle(), out_collisionMaybe);
}

bool HasCollisionCirclevsAABB(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB
	, Collision* out_collisionMaybe)
{
	bool result = HasCollisionAABBvsCircle(transformB, colB.GetAABB(), transformA.GetPosition(), colA.GetCircle(), out_collisionMaybe);

	if (result && out_collisionMaybe)
	{
		out_collisionMaybe->normal = out_collisionMaybe->normal * -1;
	}

	return result;
}

/********************************************************************************************************/

using HasCollisionFn = bool (*)(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB
	, Collision* out_collisionMaybe);

static const HasCollisionFn hasCollisionFns[ColliderShapeType::Count * ColliderShapeType::Count] =
{
					/* AABB */					/* Circle */
	/* AABB */		HasCollisionAABBvsAABB,		HasCollisionAABBvsCircle,
	/* Circle */	HasCollisionCirclevsAABB,	HasCollisionCirclevsCircle,
};

bool CollisionFunctions::HasCollision(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB
	, Collision* out_collisionMaybe)
{
	HasCollisionFn fn = hasCollisionFns[colA.GetShapeType() * ColliderShapeType::Count + colB.GetShapeType()];
	return fn ? fn(transformA, colA, transformB, colB, out_collisionMaybe) : false;
}
