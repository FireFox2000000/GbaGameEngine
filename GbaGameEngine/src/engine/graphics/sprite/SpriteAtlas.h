#pragma once

#ifdef Platform_GBA
#include "engine/gba/graphics/sprite/GBASpriteAtlasGfxData.h"
#endif

template<typename T>
class IPool;
class MemoryMappedFileStream;
class Sprite;
struct SpriteNode;

// Container for a list of sprites that are on the same atlas. 
// Sprites on the same atlas will share a colour palette between them, reducing palette memory usage.
class SpriteAtlas
{		
	SpriteNode* m_spritesLLHead = nullptr;
	int m_assetHash = -1;
#ifdef Platform_GBA
	GBA::Gfx::SpriteAtlasGfxData m_gfxData;
#endif
public:
	SpriteAtlas();

	SpriteNode* GetHead() const;

	// This is slow, cache when possible
	Sprite* GetSprite(int index);

	// This is slow, cache when possible
	const Sprite* GetSprite(int index) const;

	const int GetAssetHash() const { return m_assetHash; }

#ifdef Platform_GBA
	static SpriteAtlas* CreateFromFile(MemoryMappedFileStream& istream, IPool<SpriteAtlas>* spriteAtlasPool, IPool<SpriteNode>* spriteNodePool);

	const GBA::Gfx::SpriteAtlasGfxData& GetGfxData() const { return m_gfxData; }
	GBA::Gfx::SpriteAtlasGfxData& EditGfxData() { return m_gfxData; }
#endif
};