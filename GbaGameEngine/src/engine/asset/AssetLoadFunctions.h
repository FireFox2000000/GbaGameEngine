#pragma once
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"

class Engine;

namespace AssetLoadFunctions
{
	using namespace GBA::Gfx;

	SpriteAtlus CreateSpriteAtlusFromFile(const u32* file);
	TilemapSet CreateTilemapSetFromFile(const u32* file);

	void Unload(Engine* engine, TilemapSet* begin, TilemapSet* end);
}
