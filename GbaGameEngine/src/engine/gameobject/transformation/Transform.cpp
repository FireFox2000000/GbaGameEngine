#include "Transform.h"

const tFixedPoint8 DegreesToRot = tFixedPoint8(0xFFFF / 360.f);

tFixedPoint8 Component::Transform::GetRotationDegrees() const
{
	return m_localRotationDegrees;
}

void Component::Transform::SetRotationDegrees(tFixedPoint8 degrees)
{
	m_localRotationDegrees = degrees;
}

u16 Component::Transform::GetU16Rotation() const
{
	return (m_localRotationDegrees * DegreesToRot).ToRoundedInt();
}
