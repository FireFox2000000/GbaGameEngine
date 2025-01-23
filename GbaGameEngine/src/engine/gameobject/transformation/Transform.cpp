#include "Transform.h"

const FPi8 DegreesToRot = FPi8(0xFFFF / 360.f);

void Component::Transform::UpdateHasAffineTransformation()
{
	bool hasAffineTransformation = false;
	hasAffineTransformation |= m_localRotationDegrees != 0;
	hasAffineTransformation |= ((m_localScale.x >= 0) ? m_localScale.x : m_localScale.x * -1) != 1;
	hasAffineTransformation |= ((m_localScale.y >= 0) ? m_localScale.y : m_localScale.y * -1) != 1;

	m_hasAffineTrasformation = hasAffineTransformation;
}

Component::Transform::tPosition Component::Transform::GetLocalPosition() const
{
	return m_localPosition;
}

void Component::Transform::SetLocalPosition(tPosition pos)
{
	m_localPosition = pos;
}

Component::Transform::tPosition Component::Transform::GetPosition() const
{
	return m_localPosition;
}

void Component::Transform::SetPosition(tPosition pos)
{
	m_localPosition = pos;
}

Component::Transform::tScale Component::Transform::GetLocalScale() const
{
	return m_localScale;
}

void Component::Transform::SetLocalScale(tScale scale)
{
	m_localScale = scale; 
	UpdateHasAffineTransformation();
}

Component::Transform::tScale Component::Transform::GetScale() const
{
	return GetLocalScale();
}

void Component::Transform::SetScale(tScale scale)
{
	SetLocalScale(scale);
}

FPi8 Component::Transform::GetRotationDegrees() const
{
	return m_localRotationDegrees;
}

void Component::Transform::SetRotationDegrees(FPi8 degrees)
{
	m_localRotationDegrees = degrees;

	UpdateHasAffineTransformation();
}

bool Component::Transform::HasAffineTransformation() const
{
	return m_hasAffineTrasformation;
}

