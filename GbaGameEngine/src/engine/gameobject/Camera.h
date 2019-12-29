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
