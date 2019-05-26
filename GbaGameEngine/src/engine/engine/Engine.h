#pragma once

#include "engine/time/Time.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/base/ecs/EntityComponentManager.h"

class Engine
{
	Time time;

	GBA::OAMManager m_oamManager;
	SpriteManager m_spriteManager;
	ECS::EntityComponentManager m_entityComponentManager;

public:
	Engine();
	~Engine();

	void Update()
	{
		time.Update();
	}

	Time* GetTime() { return &time; }
	GBA::OAMManager* GetOAMManager() { return &m_oamManager; }
	SpriteManager* GetSpriteManager() { return &m_spriteManager; }
	ECS::EntityComponentManager* GetEntityRegistry() { return &m_entityComponentManager; }
};
