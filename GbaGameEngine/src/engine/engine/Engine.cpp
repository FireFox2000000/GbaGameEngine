#include "Engine.h"
#include "engine/audio/AudioManager.h"
#include "engine/base/ecs/ManagedEntity.h"
#include "engine/graphicalassets/Graphics.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"
#include "engine/asset/libraries/FontLibrary.h"

Engine::Engine()
{
	m_engineComponentsContainer = m_engineComponentsRegistry.CreateEntity();

	ECS::ManagedEntity::ProvideEntityManagerService(&m_entityComponentManager);

	AddComponent<Time>();
	AddComponent<Graphics>();
	AddComponent<SceneManager>();
	AddComponent<FontLibrary>();
	AddComponent<AudioManager>();
}


Engine::~Engine()
{
	m_engineComponentsRegistry.DestroyEntity(m_engineComponentsContainer);

	ECS::ManagedEntity::ProvideEntityManagerService(NULL);
}
