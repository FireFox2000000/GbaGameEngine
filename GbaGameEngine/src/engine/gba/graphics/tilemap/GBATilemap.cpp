#include "GBATilemap.h"
#include "GBATilemapSet.h"

using namespace GBA::Gfx;

Tilemap::Tilemap()
{

}

TilemapSet* GBA::Gfx::Tilemap::EditTilemapSet()
{
	return m_tilemapSet;
}

bool Tilemap::IsLoaded() const
{
	return GetMapScreenBaseBlockIndex() != GBA::VramAllocator::INVALID_SBB_ID && m_tilemapSet->IsLoaded();
}

bool GBA::Gfx::Tilemap::IsDynamicallyRendered() const
{
	return m_file.m_isDynamicallyRendered;
}

const Vector2<u8>& GBA::Gfx::Tilemap::GetSizeInTiles() const
{
	return m_file.m_sizeInTiles;
}

const TilemapSet* GBA::Gfx::Tilemap::GetTilemapSet() const
{
	return m_tilemapSet;
}

u16 GBA::Gfx::Tilemap::GetTileMapLength() const
{
	return m_file.m_tileMapDataLength;
}

const GBATEK::BackgroundTilemapEntry* GBA::Gfx::Tilemap::GetTileMapData() const
{
	return m_file.m_tileMapData;
}

GBA::VramAllocator::tScreenBaseBlockIndex GBA::Gfx::Tilemap::GetMapScreenBaseBlockIndex() const
{
	return m_renderData.m_mapSbbIndex;
}

int GBA::Gfx::Tilemap::GetAssignedBackgroundSlot() const
{
	return m_renderData.m_backgroundSlotId;
}
