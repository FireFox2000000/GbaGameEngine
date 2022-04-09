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
	void PreRender();
	
protected:
	GameObject m_mainCamera;

	virtual void Enter() {}

	/// <summary>
	/// Main game-logic. Free to completely override.
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// Post game-logic stuff like animation and physics.
	/// </summary>
	virtual void LateUpdate();

	/// <summary>
	/// Fixed-tick update. May update multiple times per frame or none per frame. Ideal for physics. 
	/// </summary>
	virtual void FixedUpdate();

	virtual void Exit() {}

	/// <summary>
	/// Actual drawing to the screen takes place here.
	/// </summary>
	virtual void Render();

public:
	Scene();
	virtual ~Scene();

	const GameObject* GetMainCamera() const { return &m_mainCamera; }
};
