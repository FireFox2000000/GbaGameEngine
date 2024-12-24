#include "SpriteAssetManager.h"

SpriteAssetManager::~SpriteAssetManager()
{
#ifdef DEBUG
	for (auto& spriteNode : m_spriteNodePool)
	{
		DEBUG_ASSERTMSG(!spriteNode.sprite.IsLoaded(), "Sprites leaked! \"oamManager.UnloadAll\" not called?");
	}
#endif
}

SpriteAssetManager::SpriteAtlus* SpriteAssetManager::CreateSpriteAtlusFromFile(const u32* file)
{
	return SpriteAtlus::CreateFromFile(file, &m_spriteAtlusPool, &m_spriteNodePool);
}

void SpriteAssetManager::UnloadSpriteAtlas(SpriteAtlus* atlas)
{
	DEBUG_LOGFORMAT("Unloading sprite atlas %d", atlas->GetAssetHash());

	auto* node = atlas->GetHead();
	while (node)
	{
		auto* next = node->next;
		m_spriteNodePool.Free(node);
		node = next;
	}

	m_spriteAtlusPool.Free(atlas);
}
