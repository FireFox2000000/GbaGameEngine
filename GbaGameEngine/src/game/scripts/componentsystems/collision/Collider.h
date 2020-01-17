#pragma once
#include "engine/math/geometry/AxisAlignedBoundingBox.h"
#include "engine/base/core/stl/Bitmask.h"
#include "engine/gameobject/transformation/Transform.h"

namespace Component
{
	struct Collider
	{
		// Game specific
		enum Tag
		{
			Default,

			Count,
		};

		AxisAlignedBoundingBox2 shape = AxisAlignedBoundingBox2();		// TODO, variable shape
		bool shapeInverted = false;			// Collision wants to contain the object, rather than push it out
		Bitmask<u32> mask = Bitmask<u32>(~0);
		Tag tag = Default;
		bool isTrigger = false;
	};
}

namespace System
{
	namespace Collision
	{
		struct Collision
		{
			Vector2<tFixedPoint8> aToBProjection;
			bool isTrigger = false;
		};

		bool DoesCollide(
			const Component::Collider& colliderA, 
			const Component::Transform::tPosition& transformA, 
			const Component::Collider& colliderB, 
			const Component::Transform::tPosition& transformB,
			Collision& out_collision);

	}
}
