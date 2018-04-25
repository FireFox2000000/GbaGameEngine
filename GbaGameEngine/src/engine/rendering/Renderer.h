#ifndef PRAGMA_ONCE_ENGINE_RENDERING_RENDERER_H
#define PRAGMA_ONCE_ENGINE_RENDERING_RENDERER_H

#include "engine/component/GameObjectComponent.h"

class GameObject;
class Camera;

class Renderer : public GameObjectComponent
{
public:
	Renderer(GameObject* gameObject);
	virtual ~Renderer() {};

	virtual void Render(Camera* camera) {};
};

#endif
