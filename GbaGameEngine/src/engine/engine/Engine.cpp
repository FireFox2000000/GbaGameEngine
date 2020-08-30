#include "Engine.h"
#include "engine/graphicalassets/Graphics.h"
#include "engine/graphicalassets/tilemap/TilemapManager.h"
#include "engine/scene/SceneManager.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"

#include "engine/time/Time.h"

#include "engine/asset/libraries/SpriteLibrary.h"
#include "engine/asset/libraries/AnimationLibrary.h"
#include "engine/asset/libraries/FontLibrary.h"
#include "engine/asset/libraries/TilemapLibrary.h"

Engine::Engine()
{
	m_engineComponentsContainer = m_entityComponentManager.CreateEntity();
	ECS::ManagedEntity::ProvideEntityManagerService(&m_entityComponentManager);

	AddComponent<Time>();
	AddComponent<Graphics>();
	AddComponent<TilemapManager>();
	AddComponent<SceneManager>();

	// Asset managers
	AddComponent<SpriteLibrary>();
	AddComponent<AnimationLibrary>(EditComponent<SpriteLibrary>());
	AddComponent<FontLibrary>();
	AddComponent<TilemapLibrary>();
}


Engine::~Engine()
{
	m_entityComponentManager.DestroyEntity(m_engineComponentsContainer);
	ECS::ManagedEntity::ProvideEntityManagerService(NULL);
}
