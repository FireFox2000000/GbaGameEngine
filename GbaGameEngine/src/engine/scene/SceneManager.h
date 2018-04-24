#ifndef PRAGMA_ONCE_ENGINE_SCENE_SCENEMANAGER_H
#define PRAGMA_ONCE_ENGINE_SCENE_SCENEMANAGER_H

#include "Scene.h"

class SceneManager
{
	Scene* m_current;

public:
	SceneManager(Scene* initialScene);
	~SceneManager();

	inline void UpdateScene() { m_current->Update(); }
	inline void RenderScene() { m_current->Render(); }

};

#endif
