#ifndef PRAGMA_ONCE_ENGINE_SCENE_SCENE_H
#define PRAGMA_ONCE_ENGINE_SCENE_SCENE_H

#include "engine\gameobject\GameObject.h"
#include "engine\gameobject\Camera.h"
#include "engine\base\core\stl\List.h"

class Renderer;
class Scene
{
	friend class SceneManager;

	void Update();
	void Render();

protected:
	Camera mainCamera;
	List<GameObject> gameObjects;
	List<Renderer*> renderList;

public:
	Scene();
	virtual ~Scene();
};

#endif
