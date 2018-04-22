#include "Camera.h"

Camera::Camera()
	: m_projectionType(Projection::Orthographic)
{
}


Camera::~Camera()
{
}

Projection::Enum Camera::GetProjection() const
{
	return m_projectionType;
}

void Camera::SetProjection(Projection::Enum projection)
{
	m_projectionType = projection;
}
