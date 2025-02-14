#pragma once
#include "engine/math/geometry/AxisAlignedBoundingBox.h"

// Represents what we want to see in our world. 
// Currently only supports Orthographic display modes.
class Camera
{
public:
	enum class Projection
	{
		Orthographic
	};

private:
	Projection m_projectionType;

public:
	Camera();
	~Camera();

	Projection GetProjection() const;
	void SetProjection(Projection projection);

	AxisAlignedBoundingBox2 GetOrthoBounds() const;
};
