#ifndef PRAGMA_ONCE_ENGINE_ENGINE_ENGINE_H
#define PRAGMA_ONCE_ENGINE_ENGINE_ENGINE_H

#include "engine\gba\graphics\oam\GBAOAMManager.h"
#include "engine\graphicalassets\sprite\SpriteLoader.h"

class Engine
{
	GBA::OAMManager m_oamManager;
	SpriteLoader m_spriteLoader;

public:
	Engine();
	~Engine();

	GBA::OAMManager* GetOAMManager() { return &m_oamManager; }
	SpriteLoader* GetSpriteLoader() { return &m_spriteLoader; }
};

#endif
