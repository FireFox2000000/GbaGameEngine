#pragma once

#include "Scene.h"

class SceneManager
{
	Scene* m_current = nullptr;

public:
	SceneManager();
	~SceneManager();

	inline void UpdateScene(Engine* engine) { m_current->Update(engine); }
	inline void RenderScene(Engine* engine) { m_current->Render(engine); }

	template<typename SCENE>
	void ChangeScene(Engine* engine)
	{
		STATIC_ASSERT(IS_BASE_OF(Scene, SCENE), "SceneManager::Change must be provided a type that derives from Scene.h");

		if (m_current)
			m_current->Exit(engine);

		delete m_current;

		m_current = new SCENE(engine);

		m_current->Enter(engine);
	}
};
