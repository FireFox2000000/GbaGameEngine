#pragma once

#include "engine/time/Time.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"

class Engine
{
	Time time;

	GBA::OAMManager m_oamManager;
	SpriteManager m_spriteManager;

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
};
