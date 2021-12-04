#include "CollisionFunctions.h"
#include "Collider.h"
#include "engine/gameobject/transformation/Transform.h"

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
	, AxisAlignedBoundingBox2 colB)
{
	colA = AdjustAABBByTransform(transformA, colA);
	colB = AdjustAABBByTransform(transformB, colB);

	return colA.IntersectsOrTouches(colB);
}

bool HasCollisionCirclevsCircle(
	const Vector2<tFixedPoint8>& positionA
	, Circle colA
	, const Vector2<tFixedPoint8>& positionB
	, Circle colB)
{
	tFixedPoint8 lengthSqrd = Vector2<tFixedPoint8>::LengthSqrd(positionA, positionB);
	tFixedPoint8 rad = colA.radius + colB.radius;
	tFixedPoint8 radSqrd = rad * rad;

	return radSqrd >= lengthSqrd;
}

bool HasCollisionAABBvsCircle(
	const Component::Transform& transformA
	, AxisAlignedBoundingBox2 colA
	, const Vector2<tFixedPoint8>& positionB
	, Circle colB)
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

	return delta.MagnitudeSqrd() < (colB.radius * colB.radius);
}

bool HasCollisionAABBvsAABB(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB)
{
	return HasCollisionAABBvsAABB(transformA, colA.GetAABB(), transformB, colB.GetAABB());
}

/********************************************************************************************************/

bool HasCollisionCirclevsCircle(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB)
{
	return HasCollisionCirclevsCircle(transformA.GetPosition(), colA.GetCircle(), transformA.GetPosition(), colB.GetCircle());
}

bool HasCollisionAABBvsCircle(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB)
{
	return HasCollisionAABBvsCircle(transformA, colA.GetAABB(), transformB.GetPosition(), colB.GetCircle());
}

bool HasCollisionCirclevsAABB(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB)
{
	return HasCollisionAABBvsCircle(transformB, colB.GetAABB(), transformA.GetPosition(), colA.GetCircle());
}

/********************************************************************************************************/

using HasCollisionFn = bool (*)(
	const Component::Transform& transformA
	, const Component::Collider& colA
	, const Component::Transform& transformB
	, const Component::Collider& colB);

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
	, const Component::Collider& colB)
{
	HasCollisionFn fn = hasCollisionFns[colA.GetShapeType() * ColliderShapeType::Count + colB.GetShapeType()];
	return fn ? fn(transformA, colA, transformB, colB) : false;
}
