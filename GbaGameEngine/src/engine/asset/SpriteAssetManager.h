#pragma once
#include "engine/gba/graphics/sprite/GBASpriteAtlas.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"
#include "engine/base/core/stl/Pool.h"

// Manages a collection of sprite atlas' and sprites. 
// Sprite atlas' do not own sprites anymore, rather they are stitched together in a linked list. 
// These sprites are created from an external pool, created in this manager.
// So both these objects are owned by, and should only ever be created by this manager.
class SpriteAssetManager
{
	using SpriteAtlas  = GBA::Gfx::SpriteAtlas;
	using SpriteNode = GBA::Gfx::SpriteNode;

	Pool<SpriteAtlas, 4> m_spriteAtlasPool;
	Pool<SpriteNode, 32> m_spriteNodePool;

public:
	SpriteAssetManager() = default;
	~SpriteAssetManager();

	// Non-copyable, must keep linked list intact
	SpriteAssetManager& operator=(const SpriteAssetManager&) = delete;
	SpriteAssetManager(const SpriteAssetManager&) = delete;

	SpriteAtlas* CreateSpriteAtlasFromFile(const u32* file);
	void UnloadSpriteAtlas(SpriteAtlas* atlas);
};
