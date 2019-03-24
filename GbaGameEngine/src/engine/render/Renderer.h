#pragma once

#include "engine/component/GameObjectComponent.h"

class GameObject;
class Camera;
class Engine;

class Renderer : public GameObjectComponent
{
public:
	Renderer(GameObject* gameObject);
	virtual ~Renderer() {};

	virtual void Render(Engine* engine, Camera* camera) {};
};
