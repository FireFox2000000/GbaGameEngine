#pragma once

#include "Scene.h"

// State machine specific for scene management
class SceneManager
{
	using CreateScene = std::function<Scene*(Engine*)>;

	Scene* m_current = nullptr;
	CreateScene m_queuedSceneFn = nullptr;


public:
	SceneManager();
	~SceneManager();

	void UpdateScene(Engine* engine);
	inline void PreRenderScene(Engine* engine) { m_current->PreRender(engine); }
	inline void RenderScene(Engine* engine) { m_current->Render(engine); }

	bool EnterQueuedScene(Engine* engine);

	template<typename SCENE>
	void ChangeScene(Engine* engine)
	{
		STATIC_ASSERT(IS_BASE_OF(Scene, SCENE), "SceneManager::Change must be provided a type that derives from Scene.h");

		// We queue up a scene load to make sure we aren't changing a scene while still running update functionality from that same scene
		m_queuedSceneFn = [](Engine* engine) { return new SCENE(engine); };

		if (!m_current)
			EnterQueuedScene(engine);	// Initial scene
	}
};
