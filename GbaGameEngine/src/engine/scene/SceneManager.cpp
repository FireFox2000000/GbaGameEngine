#include "SceneManager.h"
#include "engine/base/Macros.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	if (m_current)
		delete m_current;
}

void SceneManager::UpdatedQueuedChange(Engine * engine)
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

		m_current->Enter(engine);
	}
}

void SceneManager::UpdateScene(Engine * engine)
{
	UpdatedQueuedChange(engine);

	if (m_current)
		m_current->Update(engine);
}

