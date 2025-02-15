#include "ResourcesManager.h"
#include "engine/io/filestream/MemoryMappedFileStream.h"
#include "engine/asset/AssetLoadFunctions.h"
#include "engine/engine/Engine.h"
#include "engine/graphics/Graphics.h"

ResourcesManager::~ResourcesManager()
{
#ifdef DEBUG
	for (auto& spriteNode : m_spriteNodePool)
	{
		DEBUG_ASSERTMSG(!spriteNode.sprite.IsLoaded(), "Sprites leaked! \"oamManager.UnloadAll\" not called?");
	}

	for (auto& resource : m_spriteAnimationPool)
	{
		DEBUG_ASSERTMSG(false, "Sprite animations leaked, missed call to ResourcesManager::Unload(SpriteAnimation* ...)");
	}

	for (auto& resource : m_tilemapSetPool)
	{
		DEBUG_ASSERTMSG(false, "Tilemap set leaked, missed call to ResourcesManager::Unload(TilemapSet* ...)");
	}
#endif
}

SpriteAtlas* ResourcesManager::LoadSpriteAtlas(const MemoryMappedFileView file)
{
	MemoryMappedFileStream istream(file);
	auto* atlas = SpriteAtlas::CreateFromFile(istream, &m_spriteAtlasPool, &m_spriteNodePool);
	
	DEBUG_LOGFORMAT("Loaded sprite atlas %d", atlas->GetAssetHash());
	DEBUG_LOGFORMAT("Sprite atlas pool capacity %d", m_spriteAtlasPool.Capacity());
	DEBUG_LOGFORMAT("Sprite node pool capacity %d", m_spriteNodePool.Capacity());

	return atlas;
}

void ResourcesManager::Unload(SpriteAtlas* atlas)
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

SpriteAnimation* ResourcesManager::CreateSpriteAnimation(const SpriteAnimation& animation)
{
	return m_spriteAnimationPool.Create(animation);
}

void ResourcesManager::Unload(SpriteAnimation* animation)
{
	m_spriteAnimationPool.Free(animation);
}

GBA::Gfx::TilemapSet* ResourcesManager::LoadTilemapSet(const MemoryMappedFileView file)
{
	MemoryMappedFileStream istream(file);

	GBA::Gfx::TilemapSet* tilemapSet = AssetLoadFunctions::CreateTilemapSetFromFile(istream, m_tilemapSetPool);
	return tilemapSet;
}

void ResourcesManager::Unload(GBA::Gfx::TilemapSet* tilemapSet)
{
	Graphics* graphicsManager = Engine::GetInstance().GetComponent<Graphics>();

	for (auto& tilemap : tilemapSet->m_maps)
	{
		graphicsManager->Unload(&tilemap);
	}
}
