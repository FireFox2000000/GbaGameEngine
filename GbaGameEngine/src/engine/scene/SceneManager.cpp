#include "SceneManager.h"
#include "engine/base/Macros.h"
#include "Scene.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	if (m_current)
		delete m_current;
}

bool SceneManager::EnterQueuedScene(Engine * engine)
{
	if (m_queuedSceneFn)
	{
		if (m_current)
		{
			m_current->Exit(engine);
			delete m_current;
		}

		m_current = m_queuedSceneFn(engine);
		m_queuedSceneFn = nullptr;

		DEBUG_LOG("Scene changed");

		m_current->Enter(engine);

		return true;
	}

	return false;
}

void SceneManager::UpdateScene(Engine * engine)
{
	if (m_current)
	{
		m_current->Update(engine);
		m_current->LateUpdate(engine);
	}
}

void SceneManager::PreRenderScene(Engine* engine)
{
	m_current->PreRender(engine);
}

void SceneManager::RenderScene(Engine* engine)
{
	m_current->Render(engine);
}

