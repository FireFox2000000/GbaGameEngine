#pragma once

#include "engine/gameobject/GameObject.h"
#include "engine/base/core/stl/List.h"

class Renderer;
class Engine;
class Scene
{
	friend class SceneManager;

	void PreRender(Engine* engine);
	void Render(Engine* engine);

protected:
	GameObject m_mainCamera;

	virtual void Enter(Engine* engine) {}
	virtual void Update(Engine* engine);
	virtual void Exit(Engine* engine) {}

public:
	Scene(Engine* engine);
	virtual ~Scene();
};
