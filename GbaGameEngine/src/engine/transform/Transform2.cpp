#include "Transform2.h"

Vector2<FPi16> Transform2::GetLocalPosition() const
{
	return m_localPosition;
}

void Transform2::SetLocalPosition(Vector2<FPi16> pos)
{
	m_localPosition = pos;
}

Vector2<FPi16> Transform2::GetPosition() const
{
	return m_localPosition;
}

void Transform2::SetPosition(Vector2<FPi16> pos)
{
	m_localPosition = pos;
}

Vector2<FPi16> Transform2::GetLocalScale() const
{
	return m_localScale;
}

void Transform2::SetLocalScale(Vector2<FPi16> scale)
{
	m_localScale = scale; 
}

Vector2<FPi16> Transform2::GetScale() const
{
	return GetLocalScale();
}

void Transform2::SetScale(Vector2<FPi16> scale)
{
	SetLocalScale(scale);
}

FPi16 Transform2::GetRotationDegrees() const
{
	return m_localRotationDegrees;
}

void Transform2::SetRotationDegrees(FPi16 degrees)
{
	m_localRotationDegrees = degrees;
}
