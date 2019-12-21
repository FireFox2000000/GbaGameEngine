#include "Engine.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/scene/SceneManager.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/time/Time.h"

#include "engine/asset/libraries/SpriteLibrary.h"
#include "engine/asset/libraries/AnimationLibrary.h"

Engine::Engine()
{
	m_engineComponentsContainer = m_entityComponentManager.CreateEntity();

	AddComponent<Time>();
	AddComponent<SpriteManager>();
	AddComponent<SceneManager>();
	AddComponent<GBA::OAMManager>();

	// Asset managers
	AddComponent<SpriteLibrary>();
	AddComponent<AnimationLibrary>(EditComponent<SpriteLibrary>());
}


Engine::~Engine()
{
	m_entityComponentManager.DestroyEntity(m_engineComponentsContainer);
}
