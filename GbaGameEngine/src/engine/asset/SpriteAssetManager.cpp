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

SpriteAssetManager::SpriteAtlas* SpriteAssetManager::CreateSpriteAtlasFromFile(const u32* file)
{
	auto* atlas = SpriteAtlas::CreateFromFile(file, &m_spriteAtlasPool, &m_spriteNodePool);
	
	DEBUG_LOGFORMAT("Loaded sprite atlas %d", atlas->GetAssetHash());
	DEBUG_LOGFORMAT("Sprite atlas pool capacity %d", m_spriteAtlasPool.Capacity());
	DEBUG_LOGFORMAT("Sprite node pool capacity %d", m_spriteNodePool.Capacity());

	return atlas;
}

void SpriteAssetManager::UnloadSpriteAtlas(SpriteAtlas* atlas)
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

	m_spriteAtlasPool.Free(atlas);
}

SpriteAnimation* SpriteAssetManager::CreateSpriteAnimation(const SpriteAnimation& animation)
{
	return m_spriteAnimationPool.Create(animation);
}

void SpriteAssetManager::UnloadSpriteAnimation(SpriteAnimation* animation)
{
	m_spriteAnimationPool.Free(animation);
}
