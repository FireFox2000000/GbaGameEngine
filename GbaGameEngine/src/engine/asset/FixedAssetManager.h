#pragma once
#include "engine/gba/graphics/sprite/GBASprite.h"
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"

#include "engine/base/core/stl/List.h"
#include "engine/asset/AssetLoadFunctions.h"

using Sprite = GBA::Gfx::Sprite;
using SpriteAtlas = GBA::Gfx::SpriteAtlas;
using TilemapSet = GBA::Gfx::TilemapSet;
using Tilemap = GBA::Gfx::Tilemap;

class Engine;

template<typename TilemapSetEnums>
class FixedAssetManager
{
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

	TilemapSet* AddTilemapSetFromFile(TilemapSetEnums e, const u32* file);
	Tilemap* GetTilemap(TilemapSetEnums tilemapId, u32 tilemapIndex) { return m_tilemapSets[tilemapId].GetTilemap(tilemapIndex); }
};

template<typename TilemapSetEnums>
inline TilemapSet* FixedAssetManager<TilemapSetEnums>::AddTilemapSetFromFile(TilemapSetEnums e, const u32 * file)
{
	auto tilemapSet = AssetLoadFunctions::CreateTilemapSetFromFile(file);
	return new(&m_tilemapSets[e]) TilemapSet(tilemapSet);
}
