#include "Camera.h"
#include "engine/gba/graphics/tiles/GBATile.h"
#include "engine/screen/Screen.h"

Camera::Camera()
	: m_projectionType(Projection::Orthographic)
{
}

Camera::~Camera()
{
}

Camera::Projection Camera::GetProjection() const
{
	return m_projectionType;
}

void Camera::SetProjection(Projection projection)
{
	m_projectionType = projection;
}

AxisAlignedBoundingBox2 Camera::GetOrthoBounds() const
{
	const Vector2<FPi16> screenSpaceOffset = Vector2<FPi16>(Screen::GetResolution() / 2);
	const Vector2<FPi16> worldSpaceOffset = screenSpaceOffset / GBA::Gfx::Tile::PIXELS_SQRROOT_PER_TILE;
	return AxisAlignedBoundingBox2(worldSpaceOffset * -1, worldSpaceOffset);
}
