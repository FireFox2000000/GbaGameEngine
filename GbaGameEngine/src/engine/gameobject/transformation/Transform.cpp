#include "Transform.h"

const tFixedPoint8 DegreesToRot = tFixedPoint8(0xFFFF / 360.f);

void Component::Transform::UpdateHasAffineTransformation()
{
	bool hasAffineTransformation = false;
	hasAffineTransformation |= m_localRotationDegrees != 0;
	hasAffineTransformation |= ((m_localScale.x >= 0) ? m_localScale.x : m_localScale.x * -1) != 1;
	hasAffineTransformation |= ((m_localScale.y >= 0) ? m_localScale.y : m_localScale.y * -1) != 1;

	m_hasAffineTrasformation = hasAffineTransformation;
}

tFixedPoint8 Component::Transform::GetRotationDegrees() const
{
	return m_localRotationDegrees;
}

void Component::Transform::SetRotationDegrees(tFixedPoint8 degrees)
{
	m_localRotationDegrees = degrees;

	UpdateHasAffineTransformation();
}

u16 Component::Transform::GetU16Rotation() const
{
	return (m_localRotationDegrees * DegreesToRot).ToRoundedInt();
}
