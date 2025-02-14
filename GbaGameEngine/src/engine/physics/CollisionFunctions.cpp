#include "CollisionFunctions.h"
#include "Collider.h"
#include "Collision.h"
#include "engine/transform/Transform2.h"
#include "engine/math/VectorMath.h"

// Helpful resources for future self:
// https://noonat.github.io/intersect/
// https://www.gamasutra.com/view/feature/3383/simple_intersection_tests_for_games.php?print=1

inline AxisAlignedBoundingBox2 AdjustAABBByTransform(const Transform2& transformA, AxisAlignedBoundingBox2 aabb)
{
	// TODO, maybe AABB should only adjust by position? And use OBB instead when doing these kinds of transforms? Depends on the game I guess. 
	// AABB with scale transformations applies would be cheaper than full OBB checks

	auto scale = transformA.GetScale();

	Vector2<FPi16> min = aabb.min;
	Vector2<FPi16> max = aabb.max;

	min.x *= scale.x;
	min.y *= scale.y;
	max.x *= scale.x;
	max.y *= scale.y;

	Vector2<FPi16> newMinA(FPi16(MIN(min.x, max.x)), FPi16(MIN(min.y, max.y)));
	Vector2<FPi16> newMaxA(FPi16(MAX(min.x, max.x)), FPi16(MAX(min.y, max.y)));

	aabb = AxisAlignedBoundingBox2(newMinA, newMaxA);

	aabb.Translate(transformA.GetPosition());

	return aabb;
}

/********************************************************************************************************/

bool HasCollisionAABBvsAABB(
	const Transform2& transformA
	, AxisAlignedBoundingBox2 colA
	, const Transform2& transformB
	, AxisAlignedBoundingBox2 colB
	, Collision* out_collisionMaybe)
{
	colA = AdjustAABBByTransform(transformA, colA);
	colB = AdjustAABBByTransform(transformB, colB);

	bool result = colA.IntersectsOrTouches(colB);

	if (result && out_collisionMaybe)
	{
		Vector2<FPi16> halfExtentsA = (colA.max - colA.min) * FPi16(0.5f);
		Vector2<FPi16> aabbCenterA = colA.min + halfExtentsA;

		Vector2<FPi16> halfExtentsB = (colB.max - colB.min) * FPi16(0.5f);
		Vector2<FPi16> aabbCenterB = colB.min + halfExtentsB;

		FPi16 dx = aabbCenterB.x - aabbCenterA.x;
		FPi16 px = (halfExtentsB.x + halfExtentsA.x) - Math::Abs(dx);

		FPi16 dy = aabbCenterB.y - aabbCenterA.y;
		FPi16 py = (halfExtentsB.y + halfExtentsA.y) - Math::Abs(dy);

		if (px < py) 
		{
			FPi16 sx = Math::Sign(dx);
			out_collisionMaybe->penetrationDepth = Math::Abs(px);
			out_collisionMaybe->normal = { sx * -1, 0 };
		}
		else 
		{
			FPi16 sy = Math::Sign(dy);
			out_collisionMaybe->penetrationDepth = Math::Abs(FPi16(py));
			out_collisionMaybe->normal = { 0, sy * -1 };
		}
	}

	return result;
}

bool HasCollisionCirclevsCircle(
	const Vector2<FPi16>& positionA
	, Circle colA
	, const Vector2<FPi16>& positionB
	, Circle colB
	, Collision* out_collisionMaybe)
{
	FPi16 lengthSqrd = VectorMath::LengthSqrd(positionA, positionB);
	FPi16 rad = colA.radius + colB.radius;
	FPi16 radSqrd = rad * rad;

	bool result = radSqrd >= lengthSqrd;

	if (result && out_collisionMaybe)
	{
		Vector2<FPi16> direction = positionA - positionB;
		Vector2<FPi16> normal = Vector2<FPi16>(VectorMath::Normalised(Vector2f(direction)));

		out_collisionMaybe->normal = normal;
		out_collisionMaybe->penetrationDepth = rad - FPi16(std::sqrt(static_cast<float>(lengthSqrd)));	// float heavy, but need accuracy or this doesn't work.
	}

	return result;
}

bool HasCollisionAABBvsCircle(
	const Transform2& transformA
	, AxisAlignedBoundingBox2 colA
	, const Vector2<FPi16>& positionB
	, Circle colB
	, Collision* out_collisionMaybe)
{
	// https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-Detection - AABB - Circle collision detection

	colA = AdjustAABBByTransform(transformA, colA);
	Vector2<FPi16> halfExtents = (colA.max - colA.min) * FPi16(0.5f);
	Vector2<FPi16> aabbCenter = colA.min + halfExtents;

	Vector2<FPi16> delta = positionB - aabbCenter;
	Vector2<FPi16> clamped(
		MAX(FPi16(-1) * halfExtents.x, MIN(halfExtents.x, delta.x)),
		MAX(FPi16(-1) * halfExtents.y, MIN(halfExtents.y, delta.y))
		);

	Vector2<FPi16> closestPoint = aabbCenter + clamped;

	delta = closestPoint - positionB;

	bool result = delta.MagnitudeSqrd() < (colB.radius * colB.radius);

	if (result && out_collisionMaybe)
	{
		Vector2<FPi16> direction = closestPoint - positionB;
		Vector2<FPi16> normal = Vector2<FPi16>(VectorMath::Normalised(Vector2f(direction)));
		
		out_collisionMaybe->normal = normal;
		out_collisionMaybe->penetrationDepth = colB.radius - FPi16(std::sqrt(static_cast<float>(direction.MagnitudeSqrd())));	// float heavy, but need accuracy or this doesn't work.		
	}

	return result;
}

bool HasCollisionAABBvsAABB(
	const Transform2& transformA
	, const Collider& colA
	, const Transform2& transformB
	, const Collider& colB
	, Collision* out_collisionMaybe)
{
	return HasCollisionAABBvsAABB(transformA, colA.GetAABB(), transformB, colB.GetAABB(), out_collisionMaybe);
}

/********************************************************************************************************/

bool HasCollisionCirclevsCircle(
	const Transform2& transformA
	, const Collider& colA
	, const Transform2& transformB
	, const Collider& colB
	, Collision* out_collisionMaybe)
{
	return HasCollisionCirclevsCircle(transformA.GetPosition(), colA.GetCircle(), transformB.GetPosition(), colB.GetCircle(), out_collisionMaybe);
}

bool HasCollisionAABBvsCircle(
	const Transform2& transformA
	, const Collider& colA
	, const Transform2& transformB
	, const Collider& colB
	, Collision* out_collisionMaybe)
{
	return HasCollisionAABBvsCircle(transformA, colA.GetAABB(), transformB.GetPosition(), colB.GetCircle(), out_collisionMaybe);
}

bool HasCollisionCirclevsAABB(
	const Transform2& transformA
	, const Collider& colA
	, const Transform2& transformB
	, const Collider& colB
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
	const Transform2& transformA
	, const Collider& colA
	, const Transform2& transformB
	, const Collider& colB
	, Collision* out_collisionMaybe);

static const HasCollisionFn hasCollisionFns[static_cast<int>(ColliderShapeType::Count) * static_cast<int>(ColliderShapeType::Count)] =
{
					/* AABB */					/* Circle */
	/* AABB */		HasCollisionAABBvsAABB,		HasCollisionAABBvsCircle,
	/* Circle */	HasCollisionCirclevsAABB,	HasCollisionCirclevsCircle,
};

bool CollisionFunctions::HasCollision(
	const Transform2& transformA
	, const Collider& colA
	, const Transform2& transformB
	, const Collider& colB
	, Collision* out_collisionMaybe)
{
	HasCollisionFn fn = hasCollisionFns[static_cast<int>(colA.GetShapeType()) * static_cast<int>(ColliderShapeType::Count) + static_cast<int>(colB.GetShapeType())];
	return fn ? fn(transformA, colA, transformB, colB, out_collisionMaybe) : false;
}
