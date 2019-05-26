#include "Camera.h"

Component::Camera::Camera()
	: m_projectionType(Projection::Orthographic)
{
}


Component::Camera::~Camera()
{
}

Projection::Enum Component::Camera::GetProjection() const
{
	return m_projectionType;
}

void Component::Camera::SetProjection(Projection::Enum projection)
{
	m_projectionType = projection;
}
