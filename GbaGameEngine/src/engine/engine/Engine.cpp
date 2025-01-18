#include "Engine.h"
#include "engine/audio/AudioManager.h"
#include "engine/base/ecs/ManagedEntity.h"
#include "engine/input/InputManager.h"
#include "engine/io/FileSystem.h"
#include "engine/graphics/Graphics.h"
#include "engine/scene/SceneManager.h"
#include "engine/time/Time.h"

Engine::Engine()
	: m_audioManager(new AudioManager())
	, m_graphics(new Graphics())
{
	m_engineComponentsContainer = m_engineComponentsRegistry.CreateEntity();

	ECS::ManagedEntity::ProvideEntityManagerService(&m_entityComponentManager);

	AddComponent<Time>();
	AddComponent<SceneManager>();
	AddComponent<IO::FileSystem>();
	AddComponent<Input::InputManager>();
}


Engine::~Engine()
{
	delete m_audioManager;
	delete m_graphics;

	m_engineComponentsRegistry.DestroyEntity(m_engineComponentsContainer);

	ECS::ManagedEntity::ProvideEntityManagerService(nullptr);
}
