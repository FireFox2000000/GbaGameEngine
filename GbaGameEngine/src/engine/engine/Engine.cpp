#include "Engine.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/scene/SceneManager.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/time/Time.h"

Engine::Engine()
{
	m_engineComponentsContainer = m_entityComponentManager.CreateEntity();

	AddComponent<Time>();
	AddComponent<SpriteManager>();
	AddComponent<SceneManager>();
	AddComponent<GBA::OAMManager>();
}


Engine::~Engine()
{
	m_entityComponentManager.DestroyEntity(m_engineComponentsContainer);
}
