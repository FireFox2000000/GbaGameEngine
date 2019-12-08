#pragma once

#include "engine/gameobject/GameObject.h"
#include "engine/base/core/stl/List.h"

class Renderer;
class Engine;
class Scene
{
	friend class SceneManager;

	void Render(Engine* engine);

protected:
	GameObject m_mainCamera;
	List<GameObject> m_gameObjects;

	virtual void Enter(Engine* engine) {}
	virtual void Update(Engine* engine);
	virtual void Exit(Engine* engine) {}

public:
	Scene(Engine* engine);
	virtual ~Scene();
};
