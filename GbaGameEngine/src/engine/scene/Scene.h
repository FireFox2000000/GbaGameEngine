#pragma once

#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/Camera.h"
#include "engine/base/core/stl/List.h"

class Renderer;
class Engine;
class Scene
{
	friend class SceneManager;

	void Update(Engine* engine);
	void Render(Engine* engine);

protected:
	Camera mainCamera;
	List<GameObject> gameObjects;
	List<Renderer*> renderList;

public:
	Scene(Engine* engine);
	virtual ~Scene();
};
