#pragma once
#include "engine/math/geometry/AxisAlignedBoundingBox.h"

namespace Projection
{
	enum Enum
	{
		Orthographic
	};
}

namespace Component
{
	// Represents what we want to see in our world. 
	// Currently only supports Orthographic display modes.
	class Camera
	{
		Projection::Enum m_projectionType;

	public:
		Camera();
		~Camera();

		Projection::Enum GetProjection() const;
		void SetProjection(Projection::Enum projection);

		AxisAlignedBoundingBox2 GetOrthoBounds() const;
	};
}
