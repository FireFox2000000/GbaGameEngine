#pragma once

class Transform2;

namespace Component
{
	class Collider;
}

struct Collision;

namespace CollisionFunctions
{
	bool HasCollision(
		const Transform2& transformA
		, const Component::Collider& colA
		, const Transform2& transformB
		, const Component::Collider& colB
		, Collision* out_collisionMaybe = nullptr);
}
