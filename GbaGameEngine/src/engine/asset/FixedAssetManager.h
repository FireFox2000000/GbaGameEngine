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

template<typename AnimationsEnum, typename TilemapSetEnums>
class FixedAssetManager
{
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
#endif
	}

	void Dispose(Engine * engine)
	{
		AssetLoadFunctions::Unload(engine, m_tilemapSets.begin(), m_tilemapSets.end());
	}

	// Asset lookups
	const SpriteAnimation* GetAsset(AnimationsEnum animationId);

	SpriteAnimation* AddSpriteAnimation(AnimationsEnum e, const SpriteAnimation& animation);
	const SpriteAnimation* GetSpriteAnimation(AnimationsEnum animationId);

	TilemapSet* AddTilemapSetFromFile(TilemapSetEnums e, const u32* file);
	Tilemap* GetTilemap(TilemapSetEnums tilemapId, u32 tilemapIndex) { return m_tilemapSets[0].GetTilemap(tilemapIndex); }
};

template<typename AnimationsEnum, typename TilemapSetEnums>
inline const SpriteAnimation * FixedAssetManager<AnimationsEnum, TilemapSetEnums>::GetAsset(AnimationsEnum animationId)
{
	return &m_spriteAnimations[animationId];
}

template<typename AnimationsEnum, typename TilemapSetEnums>
inline SpriteAnimation* FixedAssetManager< AnimationsEnum, TilemapSetEnums>::AddSpriteAnimation(AnimationsEnum e, const SpriteAnimation & animation)
{
	return new(&m_spriteAnimations[e]) SpriteAnimation(animation);
}

template<typename AnimationsEnum, typename TilemapSetEnums>
inline const SpriteAnimation * FixedAssetManager<AnimationsEnum, TilemapSetEnums>::GetSpriteAnimation(AnimationsEnum animationId)
{
	return &m_spriteAnimations[animationId];
}

template<typename AnimationsEnum, typename TilemapSetEnums>
inline TilemapSet* FixedAssetManager<AnimationsEnum, TilemapSetEnums>::AddTilemapSetFromFile(TilemapSetEnums e, const u32 * file)
{
	auto tilemapSet = AssetLoadFunctions::CreateTilemapSetFromFile(file);
	return new(&m_tilemapSets[e]) TilemapSet(tilemapSet);
}
