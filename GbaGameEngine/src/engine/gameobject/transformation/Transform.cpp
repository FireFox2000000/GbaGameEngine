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

Vector2<FPi16> Component::Transform::GetLocalPosition() const
{
	return m_localPosition;
}

void Component::Transform::SetLocalPosition(Vector2<FPi16> pos)
{
	m_localPosition = pos;
}

Vector2<FPi16> Component::Transform::GetPosition() const
{
	return m_localPosition;
}

void Component::Transform::SetPosition(Vector2<FPi16> pos)
{
	m_localPosition = pos;
}

Vector2<FPi16> Component::Transform::GetLocalScale() const
{
	return m_localScale;
}

void Component::Transform::SetLocalScale(Vector2<FPi16> scale)
{
	m_localScale = scale; 
	UpdateHasAffineTransformation();
}

Vector2<FPi16> Component::Transform::GetScale() const
{
	return GetLocalScale();
}

void Component::Transform::SetScale(Vector2<FPi16> scale)
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

