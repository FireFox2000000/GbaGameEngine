#pragma once

#include "engine/gameobject/GameObject.h"
#include "engine/base/core/stl/List.h"

class Renderer;
class Engine;

// Base class for managing game scene state. Automatically provides a camera to use. 
class Scene
{
	friend class SceneManager;

	void PreRender(Engine* engine);
	
protected:
	GameObject m_mainCamera;

	virtual void Enter(Engine* engine) {}
	virtual void Update(Engine* engine);
	virtual void Exit(Engine* engine) {}
	virtual void Render(Engine* engine);

public:
	Scene(Engine* engine);
	virtual ~Scene();

	const GameObject* GetMainCamera() const { return &m_mainCamera; }
};
