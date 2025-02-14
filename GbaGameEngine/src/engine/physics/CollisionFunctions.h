#pragma once

class Transform2;
class Collider;
struct Collision;

namespace CollisionFunctions
{
	bool HasCollision(
		const Transform2& transformA
		, const Collider& colA
		, const Transform2& transformB
		, const Collider& colB
		, Collision* out_collisionMaybe = nullptr);
}
