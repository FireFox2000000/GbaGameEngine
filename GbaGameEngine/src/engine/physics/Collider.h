#pragma once
#include "engine/math/geometry/AxisAlignedBoundingBox.h"
#include "engine/math/geometry/Circle.h"

// A collider class that will only be as big as the largest possible shape element. 
// Only holds one shape at a time

// To add a new shape:
// 1. Add a new ShapeType to enum
// 2. Add object to Shape union
// 3. Add getters and setters
// 4. Add collision functions or nullptr to CollisionFunctions.cpp hasCollisionFns array

namespace ColliderShapeType
{
	enum Enum
	{
		AABB,
		Circle,
	
		Count
	};
}

namespace Component
{
	class Collider
	{
	private:
		union Shape
		{
			AxisAlignedBoundingBox2 aabb;
			Circle circle;

			Shape() {};
			Shape(const Shape& that);
		};

		Shape m_shape;
		ColliderShapeType::Enum m_shapeType = ColliderShapeType::Count;

		bool m_isTrigger = false;		// If set to true, objects will be allowed to pass through this collider. Otherwise a physical object. 

	public:
		ColliderShapeType::Enum GetShapeType() const;

		void SetCircle(tFixedPoint8 radius);
		inline const Circle& GetCircle() const {
			DEBUG_ASSERTMSGFORMAT(m_shapeType == ColliderShapeType::Circle, "Shape is not a circle, it is %d", m_shapeType);
			return m_shape.circle;
		}

		void SetAABB(const Vector2<tFixedPoint8>& min, const Vector2<tFixedPoint8>& max);
		inline const AxisAlignedBoundingBox2& GetAABB() const {
			DEBUG_ASSERTMSGFORMAT(m_shapeType == ColliderShapeType::AABB, "Shape is not an AABB, it is %d", m_shapeType);
			return m_shape.aabb;
		}
	};
}
