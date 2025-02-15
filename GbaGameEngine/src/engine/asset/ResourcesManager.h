#pragma once
#include "engine/graphics/sprite/SpriteAtlas.h"
#include "engine/graphics/sprite/SpriteNode.h"
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"
#include "engine/animation/SpriteAnimation.h"
#include "engine/base/core/stl/Pool.h"
#include "engine/io/MemoryMappedFileView.h"

// Manages a collection of sprite atlas' and sprites. 
// Sprite atlas' do not own sprites anymore, rather they are stitched together in a linked list. 
// These sprites are created from an external pool, created in this manager.
// So both these objects are owned by, and should only ever be created by this manager.

class SpriteAtlas;
struct SpriteNode;

class ResourcesManager
{
	Pool<SpriteAtlas, 4> m_spriteAtlasPool;
	Pool<SpriteNode, 32> m_spriteNodePool;
	Pool<SpriteAnimation, 8> m_spriteAnimationPool;
	Pool<GBA::Gfx::TilemapSet, 2> m_tilemapSetPool;

public:
	ResourcesManager() = default;
	~ResourcesManager();

	SpriteAtlas* LoadSpriteAtlas(const MemoryMappedFileView file);
	void Unload(SpriteAtlas* atlas);

	SpriteAnimation* CreateSpriteAnimation(const SpriteAnimation& animation);
	void Unload(SpriteAnimation* animation);

	GBA::Gfx::TilemapSet* LoadTilemapSet(const MemoryMappedFileView file);
	void Unload(GBA::Gfx::TilemapSet* tilemapSet);
};
