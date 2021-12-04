#pragma once

namespace Component
{
	class Collider;
	class Transform;
}

namespace CollisionFunctions
{
	bool HasCollision(
		const Component::Transform& transformA
		, const Component::Collider& colA
		, const Component::Transform& transformB
		, const Component::Collider& colB);
}
