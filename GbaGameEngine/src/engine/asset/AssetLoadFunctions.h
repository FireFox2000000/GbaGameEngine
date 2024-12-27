#pragma once
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"

class Engine;
class MemoryMappedFileStream;

template<typename T>
class IPool;

namespace AssetLoadFunctions
{
	using namespace GBA::Gfx;

	TilemapSet* CreateTilemapSetFromFile(MemoryMappedFileStream& istream, IPool<TilemapSet>& allocator);

	void Unload(Engine* engine, TilemapSet* begin, TilemapSet* end);
}
