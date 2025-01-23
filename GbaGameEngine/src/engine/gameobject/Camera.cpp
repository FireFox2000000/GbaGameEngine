#include "Camera.h"
#include "engine/gba/graphics/tiles/GBATile.h"
#include "engine/screen/Screen.h"

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

AxisAlignedBoundingBox2 Component::Camera::GetOrthoBounds() const
{
	const Vector2<FPi8> screenSpaceOffset = static_cast<Vector2<FPi8>>(Screen::GetResolution() / 2);
	const Vector2<FPi8> worldSpaceOffset = screenSpaceOffset / GBA::Gfx::Tile::PIXELS_SQRROOT_PER_TILE;
	return AxisAlignedBoundingBox2(worldSpaceOffset * -1, worldSpaceOffset);
}
