#pragma once
#include <functional>

class Scene;
class Engine;

// State machine specific for scene management
class SceneManager
{
	using CreateScene = std::function<Scene*()>;

	Scene* m_current = nullptr;
	CreateScene m_queuedSceneFn = nullptr;

public:
	SceneManager();
	~SceneManager();

	void UpdateScene();
	void FixedUpdateScene();
	void PreRenderScene();
	void RenderScene();

	bool EnterQueuedScene();
	bool HasSceneChangeQueued() { return m_queuedSceneFn != nullptr; }

	template<typename SCENE>
	void ChangeScene()
	{
		//STATIC_ASSERT(IS_BASE_OF(Scene, SCENE), "SceneManager::Change must be provided a type that derives from Scene.h");

		// We queue up a scene load to make sure we aren't changing a scene while still running update functionality from that same scene
		m_queuedSceneFn = []() { return new SCENE(); };

		if (!m_current)
			EnterQueuedScene();	// Initial scene
	}

	void Dispose();

	const Scene* GetCurrentScene() const { return m_current; }
};
