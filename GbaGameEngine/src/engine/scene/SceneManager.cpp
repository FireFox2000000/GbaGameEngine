#include "SceneManager.h"
#include "engine/base/Macros.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	if (m_current)
		delete m_current;

	if (m_queuedScene)
		delete m_queuedScene;
}

void SceneManager::UpdatedQueuedChange(Engine * engine)
{
	if (m_queuedScene)
	{
		if (m_current)
		{
			m_current->Exit(engine);
			delete m_current;
		}

		m_current = m_queuedScene;
		m_queuedScene = nullptr;

		m_current->Enter(engine);
	}
}

void SceneManager::UpdateScene(Engine * engine)
{
	UpdatedQueuedChange(engine);

	if (m_current)
		m_current->Update(engine);
}

