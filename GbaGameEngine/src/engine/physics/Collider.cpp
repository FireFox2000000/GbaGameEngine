#include "Collider.h"

namespace Component
{
	Collider::Shape::Shape(const Shape& that)
	{
		*this = that;
	};

	ColliderShapeType::Enum Collider::GetShapeType() const
	{
		return (ColliderShapeType::Enum)m_shapeType;
	}

	bool Collider::GetIsTrigger() const
	{
		return m_isTrigger;
	}

	void Collider::SetIsTrigger(bool isTrigger)
	{
		m_isTrigger = isTrigger;
	}

	void Collider::SetCircle(tFixedPoint8 radius)
	{
		m_shape.circle = Circle(radius);
		m_shapeType = ColliderShapeType::Circle;
	}

	void Collider::SetAABB(const Vector2<tFixedPoint8>& min, const Vector2<tFixedPoint8>& max)
	{
		m_shape.aabb = AxisAlignedBoundingBox2(min, max);
		m_shapeType = ColliderShapeType::AABB;
	}	
}
