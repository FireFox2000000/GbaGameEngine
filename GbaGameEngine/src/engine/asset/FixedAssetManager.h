#pragma once
#include "engine/gba/graphics/sprite/GBASprite.h"
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"
#include "engine/animation/SpriteAnimation.h"

#include "engine/base/core/stl/List.h"
#include "engine/asset/AssetLoadFunctions.h"

using Sprite = GBA::Gfx::Sprite;
using SpriteAtlus = GBA::Gfx::SpriteAtlus;
using TilemapSet = GBA::Gfx::TilemapSet;
using Tilemap = GBA::Gfx::Tilemap;

class Engine;

template<typename SpritesEnum, typename AnimationsEnum, typename TilemapSetEnums>
class FixedAssetManager
{
	Array<SpriteAtlus, SpritesEnum::SpriteCount> m_spriteAtlusCollection;
	Array<SpriteAnimation, AnimationsEnum::AnimationCount> m_spriteAnimations;
	Array<TilemapSet, TilemapSetEnums::TilemapSetCount> m_tilemapSets;

public:
	FixedAssetManager() = default;
	~FixedAssetManager()
	{
#ifdef DEBUG
		for (auto& tilemapSet : m_tilemapSets)
		{
			for (auto& tilemap : tilemapSet.m_maps)
			{
				DEBUG_ASSERTMSG(!tilemap.IsLoaded(), "Tilemap leaked! \"Dispose\" not called!");
			}
		}

		for (auto& spriteAtlus : m_spriteAtlusCollection)
		{
			for (u32 i = 0; i < spriteAtlus.GetSpriteCount(); ++i)
			{
				DEBUG_ASSERTMSG(!spriteAtlus.GetSprite(i)->IsLoaded(), "Sprites leaked! \"oamManager.UnloadAll\" not called?");
			}
		}
#endif
	}

	void Dispose(Engine * engine)
	{
		AssetLoadFunctions::Unload(engine, m_tilemapSets.begin(), m_tilemapSets.end());
	}

	// Asset loading
	SpriteAtlus* AddSpriteSheetFromFile(SpritesEnum e, const u32* file);

	// Asset lookups
	SpriteAtlus* GetSpriteAtlus(SpritesEnum atlusId) { return &m_spriteAtlusCollection[atlusId]; }
	Sprite* GetSprite(SpritesEnum atlusId, u32 spriteIndex);
	const SpriteAnimation* GetAsset(AnimationsEnum animationId);

	SpriteAnimation* AddSpriteAnimation(AnimationsEnum e, const SpriteAnimation& animation);
	const SpriteAnimation* GetSpriteAnimation(AnimationsEnum animationId);

	TilemapSet* AddTilemapSetFromFile(TilemapSetEnums e, const u32* file);
	Tilemap* GetTilemap(TilemapSetEnums tilemapId, u32 tilemapIndex) { return m_tilemapSets[0].GetTilemap(tilemapIndex); }
};

template<typename SpritesEnum, typename AnimationsEnum, typename TilemapSetEnums>
inline SpriteAtlus* FixedAssetManager<SpritesEnum, AnimationsEnum, TilemapSetEnums>::AddSpriteSheetFromFile(SpritesEnum e, const u32 * file)
{
	auto sprite = AssetLoadFunctions::CreateSpriteAtlusFromFile(file);
	return new(&m_spriteAtlusCollection[e]) SpriteAtlus(sprite);
}

template<typename SpritesEnum, typename AnimationsEnum, typename TilemapSetEnums>
inline Sprite * FixedAssetManager<SpritesEnum, AnimationsEnum, TilemapSetEnums>::GetSprite(SpritesEnum atlusId, u32 spriteIndex)
{
	SpriteAtlus* atlus = GetSpriteAtlus(atlusId);
	Sprite* sprite = atlus->GetSprite(spriteIndex);

	DEBUG_ASSERTMSGFORMAT(sprite, "Unable to get sprite for atlus %d at index %d", atlusId, spriteIndex);

	return sprite;
}

template<typename SpritesEnum, typename AnimationsEnum, typename TilemapSetEnums>
inline const SpriteAnimation * FixedAssetManager<SpritesEnum, AnimationsEnum, TilemapSetEnums>::GetAsset(AnimationsEnum animationId)
{
	return &m_spriteAnimations[animationId];
}

template<typename SpritesEnum, typename AnimationsEnum, typename TilemapSetEnums>
inline SpriteAnimation* FixedAssetManager<SpritesEnum, AnimationsEnum, TilemapSetEnums>::AddSpriteAnimation(AnimationsEnum e, const SpriteAnimation & animation)
{
	return new(&m_spriteAnimations[e]) SpriteAnimation(animation);
}

template<typename SpritesEnum, typename AnimationsEnum, typename TilemapSetEnums>
inline const SpriteAnimation * FixedAssetManager<SpritesEnum, AnimationsEnum, TilemapSetEnums>::GetSpriteAnimation(AnimationsEnum animationId)
{
	return &m_spriteAnimations[animationId];
}

template<typename SpritesEnum, typename AnimationsEnum, typename TilemapSetEnums>
inline TilemapSet* FixedAssetManager<SpritesEnum, AnimationsEnum, TilemapSetEnums>::AddTilemapSetFromFile(TilemapSetEnums e, const u32 * file)
{
	auto tilemapSet = AssetLoadFunctions::CreateTilemapSetFromFile(file);
	return new(&m_tilemapSets[e]) TilemapSet(tilemapSet);
}
