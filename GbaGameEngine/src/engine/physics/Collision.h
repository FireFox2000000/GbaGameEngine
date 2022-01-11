#pragma once
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/base/ecs/Entity.h"

struct Collision
{
	// THe entities that collided
	ECS::Entity entityA;
	ECS::Entity entityB;

	// Combination of normal vector + length of collision penetration. Actual normal would be quite expensive to calculate.
	Vector2<tFixedPoint24> normal;
	tFixedPoint24 penetrationDepth;
};