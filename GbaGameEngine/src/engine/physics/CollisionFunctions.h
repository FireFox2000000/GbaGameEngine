#pragma once

namespace Component
{
	class Collider;
	class Transform;
}

struct Collision;

namespace CollisionFunctions
{
	bool HasCollision(
		const Component::Transform& transformA
		, const Component::Collider& colA
		, const Component::Transform& transformB
		, const Component::Collider& colB
		, Collision* out_collisionMaybe = nullptr);
}
