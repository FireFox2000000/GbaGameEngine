#pragma once
#include "engine/gba/graphics/sprite/GBASpriteAtlas.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"
#include "engine/animation/SpriteAnimation.h"
#include "engine/base/core/stl/Pool.h"
#include "engine/io/MemoryMappedFileView.h"

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
	Pool<SpriteAnimation, 8> m_spriteAnimationPool;

public:
	SpriteAssetManager() = default;
	~SpriteAssetManager();

	SpriteAtlas* CreateSpriteAtlasFromFile(const MemoryMappedFileView file);
	void UnloadSpriteAtlas(SpriteAtlas* atlas);

	SpriteAnimation* CreateSpriteAnimation(const SpriteAnimation& animation);
	void UnloadSpriteAnimation(SpriteAnimation* animation);
};
