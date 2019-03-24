#pragma once

#include "GameObject.h"

namespace Projection
{
	enum Enum
	{
		Orthographic
	};
}

class Camera : public GameObject
{
	Projection::Enum m_projectionType;

public:
	Camera();
	~Camera();

	Projection::Enum GetProjection() const;
	void SetProjection(Projection::Enum projection);
};
