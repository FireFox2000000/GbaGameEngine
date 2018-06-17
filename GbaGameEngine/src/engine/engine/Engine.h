#ifndef PRAGMA_ONCE_ENGINE_ENGINE_ENGINE_H
#define PRAGMA_ONCE_ENGINE_ENGINE_ENGINE_H

#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"

class Engine
{
	GBA::OAMManager m_oamManager;
	SpriteManager m_spriteManager;

public:
	Engine();
	~Engine();

	GBA::OAMManager* GetOAMManager() { return &m_oamManager; }
	SpriteManager* GetSpriteManager() { return &m_spriteManager; }
};

#endif
