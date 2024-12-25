#include "SceneManager.h"
#include "engine/base/Macros.h"
#include "engine/engine/Engine.h"
#include "Scene.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	DEBUG_ASSERTMSG(m_current == nullptr && m_queuedSceneFn == nullptr, "SceneManager::Dispose not called.");
}

bool SceneManager::EnterQueuedScene()
{
	if (m_queuedSceneFn)
	{
		if (m_current)
		{
			m_current->Exit();
			delete m_current;
		}

		auto* entityManager = Engine::GetInstance().GetEntityRegistry();
		entityManager->InternalFinaliseDestroy();

		m_current = m_queuedSceneFn();
		m_queuedSceneFn = nullptr;

		DEBUG_LOG("Scene changed");

		m_current->Enter();

		return true;
	}

	return false;
}

void SceneManager::Dispose()
{
	if (m_current)
	{
		m_current->Exit();
		delete m_current;
		m_current = nullptr;
	}

	auto* entityManager = Engine::GetInstance().GetEntityRegistry();
	entityManager->InternalFinaliseDestroy();

	m_queuedSceneFn = nullptr;
}

void SceneManager::UpdateScene()
{
	if (m_current)
	{
		m_current->Update();
		m_current->LateUpdate();
	}
}

void SceneManager::FixedUpdateScene()
{
	if (m_current)
	{
		m_current->FixedUpdate();
	}
}

void SceneManager::PreRenderScene()
{
	m_current->PreRender();
}

void SceneManager::RenderScene()
{
	m_current->Render();
}

