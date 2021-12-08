#pragma once
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"
#include "engine/base/core/stl/Pool.h"

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

	SpriteAtlus* CreateSpriteAtlusFromFile(const u32* file) { return SpriteAtlus::CreateFromFile(file, &m_spriteAtlusPool, &m_spriteNodePool); }
};
