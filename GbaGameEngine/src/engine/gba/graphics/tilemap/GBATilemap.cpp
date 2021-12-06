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

Background::ControlRegister::Size Tilemap::GetSize() const
{
	return Background::GetRegSizeFromTileSize(m_file.m_sizeInTiles.x, m_file.m_sizeInTiles.y);
}

bool Tilemap::IsLoaded() const
{
	return GetMapScreenBaseBlockIndex() != GBA::INVALID_SBB_ID && m_tilemapSet->IsLoaded();
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

const u16* GBA::Gfx::Tilemap::GetTileMapData() const
{
	return m_file.m_tileMapData;
}

GBA::tScreenBaseBlockIndex GBA::Gfx::Tilemap::GetMapScreenBaseBlockIndex() const
{
	return m_renderData.m_mapSbbIndex;
}

GBA::BackgroundControl::Backgrounds GBA::Gfx::Tilemap::GetAssignedBackgroundSlot() const
{
	return m_renderData.m_backgroundSlotId;
}
