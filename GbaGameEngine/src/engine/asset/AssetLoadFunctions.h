#pragma once
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"

class Engine;

namespace AssetLoadFunctions
{
	using namespace GBA::Gfx;

	TilemapSet CreateTilemapSetFromFile(const u32* file);

	void Unload(Engine* engine, TilemapSet* begin, TilemapSet* end);
}
