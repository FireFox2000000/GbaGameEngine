#pragma once

#include "engine/gameobject/GameObject.h"
#include "engine/base/core/stl/List.h"

class Renderer;
class Engine;

// Base class for managing game scene state. Automatically provides a camera to use. 
class Scene
{
	friend class SceneManager;

	/// <summary>
	/// Pre-calculations for rendering are done here in order to reduce the actual time we're drawing to the screen. 
	/// </summary>
	void PreRender(Engine* engine);
	
protected:
	GameObject m_mainCamera;

	virtual void Enter(Engine* engine) {}

	/// <summary>
	/// Main game-logic. Free to completely override.
	/// </summary>
	virtual void Update(Engine* engine) {};

	/// <summary>
	/// Post game-logic stuff like animation and physics.
	/// </summary>
	virtual void LateUpdate(Engine* engine);

	virtual void Exit(Engine* engine) {}

	/// <summary>
	/// Actual drawing to the screen takes place here.
	/// </summary>
	virtual void Render(Engine* engine);

public:
	Scene(Engine* engine);
	virtual ~Scene();

	const GameObject* GetMainCamera() const { return &m_mainCamera; }
};
