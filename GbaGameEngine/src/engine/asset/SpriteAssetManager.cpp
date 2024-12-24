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
	auto* atlas = SpriteAtlus::CreateFromFile(file, &m_spriteAtlusPool, &m_spriteNodePool);
	
	DEBUG_LOGFORMAT("Loaded sprite atlas %d", atlas->GetAssetHash());
	DEBUG_LOGFORMAT("Sprite atlas pool capacity %d", m_spriteAtlusPool.Capacity());
	DEBUG_LOGFORMAT("Sprite node pool capacity %d", m_spriteNodePool.Capacity());

	return atlas;
}

void SpriteAssetManager::UnloadSpriteAtlas(SpriteAtlus* atlas)
{
	int freedCount = 0;
	auto* node = atlas->GetHead();
	while (node)
	{
		auto* next = node->next;
		m_spriteNodePool.Free(node);
		node = next;
		++freedCount;
	}

	DEBUG_LOGFORMAT("Unloading sprite atlas %d and %d sprites", atlas->GetAssetHash(), freedCount);

	m_spriteAtlusPool.Free(atlas);
}
