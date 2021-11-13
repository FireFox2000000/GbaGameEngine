#include "Engine.h"
#include "engine/graphicalassets/Graphics.h"
#include "engine/scene/SceneManager.h"

#include "engine/time/Time.h"

#include "engine/asset/libraries/FontLibrary.h"

Engine::Engine()
{
	m_engineComponentsContainer = m_entityComponentManager.CreateEntity();
	ECS::ManagedEntity::ProvideEntityManagerService(&m_entityComponentManager);

	AddComponent<Time>();
	AddComponent<Graphics>();
	AddComponent<SceneManager>();
	AddComponent<FontLibrary>();
}


Engine::~Engine()
{
	m_entityComponentManager.DestroyEntity(m_engineComponentsContainer);
	ECS::ManagedEntity::ProvideEntityManagerService(NULL);
}
