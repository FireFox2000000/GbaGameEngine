#pragma once

#include "Scene.h"

class SceneManager
{
	Scene* m_current;

public:
	SceneManager(Scene* initialScene);
	~SceneManager();

	inline void UpdateScene(Engine* engine) { m_current->Update(engine); }
	inline void RenderScene(Engine* engine) { m_current->Render(engine); }

};
