#include "Transform.h"

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
}

Vector2<FPi16> Component::Transform::GetScale() const
{
	return GetLocalScale();
}

void Component::Transform::SetScale(Vector2<FPi16> scale)
{
	SetLocalScale(scale);
}

FPi16 Component::Transform::GetRotationDegrees() const
{
	return m_localRotationDegrees;
}

void Component::Transform::SetRotationDegrees(FPi16 degrees)
{
	m_localRotationDegrees = degrees;
}
