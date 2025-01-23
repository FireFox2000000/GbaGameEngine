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

	int Collider::GetCollisionMask() const
	{
		return m_collisionMask;
	}

	void Collider::SetCollisionMask(int mask)
	{
		m_collisionMask = mask;
	}

	bool Collider::GetIsTrigger() const
	{
		return m_isTrigger;
	}

	void Collider::SetIsTrigger(bool isTrigger)
	{
		m_isTrigger = isTrigger;
	}

	void Collider::SetOnHitHandler(std::function<void(const Collision&)> handler)
	{
		DEBUG_ASSERTMSG(m_onHitHandler == nullptr, "Collider hit handler already registered");
		m_onHitHandler = handler;
	}

	void Collider::OnHit(const Collision& collision) const
	{
		if (m_onHitHandler)
		{
			m_onHitHandler(collision);
		}
	}

	void Collider::SetCircle(FPi8 radius)
	{
		m_shape.circle = Circle(radius);
		m_shapeType = ColliderShapeType::Circle;
	}

	void Collider::SetAABB(const Vector2<FPi8>& min, const Vector2<FPi8>& max)
	{
		m_shape.aabb = AxisAlignedBoundingBox2(min, max);
		m_shapeType = ColliderShapeType::AABB;
	}	
}
