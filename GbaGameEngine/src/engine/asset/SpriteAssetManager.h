#pragma once
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"
#include "engine/base/core/stl/Pool.h"

template<typename T>
class IPool;

class SpriteAssetManagerHelper
{
	using SpriteAtlus = GBA::Gfx::SpriteAtlus;
	using SpriteNode = GBA::Gfx::SpriteNode;

public:
	// TODO- When file system is implemented, pass the file string through here so we can record a log of what file we're trying to load
	static SpriteAtlus* CreateSpriteAtlusFromFile(const u32* file, IPool<SpriteAtlus>* spriteAtlusPool, IPool<SpriteNode>* spriteNodePool);
};

// Manages a collection of sprite atlus' and sprites. 
// Sprite atlus' do not own sprites anymore, rather they are stitched together in a linked list. 
// These sprites are created from an external pool, created in this manager.
// So both these objects are owned by, and should only ever be created by this manager.
template<int MAX_ATLUS, int MAX_SPRITE_NODES>
class SpriteAssetManager
{
	using SpriteAtlus  = GBA::Gfx::SpriteAtlus;
	using SpriteNode = GBA::Gfx::SpriteNode;

	Pool<SpriteAtlus, MAX_ATLUS> m_spriteAtlusPool;
	Pool<SpriteNode, MAX_SPRITE_NODES> m_spriteNodePool;

public:
	SpriteAssetManager() = default;
	~SpriteAssetManager()
	{
#ifdef DEBUG
		for (auto& spriteNode : m_spriteNodePool)
		{
			DEBUG_ASSERTMSG(!spriteNode.sprite.IsLoaded(), "Sprites leaked! \"oamManager.UnloadAll\" not called?");
		}
#endif
	}

	// Non-copyable, must keep linked list intact
	SpriteAssetManager& operator=(const SpriteAssetManager&) = delete;
	SpriteAssetManager(const SpriteAssetManager&) = delete;

	SpriteAtlus* CreateSpriteAtlusFromFile(const u32* file) { return SpriteAssetManagerHelper::CreateSpriteAtlusFromFile(file, &m_spriteAtlusPool, &m_spriteNodePool); }
};
