#ifndef PRAGMA_ONCE_ENGINE_GAMEOBJECT_CAMERA_H
#define PRAGMA_ONCE_ENGINE_GAMEOBJECT_CAMERA_H

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

#endif
