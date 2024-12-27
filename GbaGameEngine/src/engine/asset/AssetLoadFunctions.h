#pragma once
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"

class Engine;
class MemoryMappedFileStream;

namespace AssetLoadFunctions
{
	using namespace GBA::Gfx;

	TilemapSet CreateTilemapSetFromFile(MemoryMappedFileStream& istream);

	void Unload(Engine* engine, TilemapSet* begin, TilemapSet* end);
}
