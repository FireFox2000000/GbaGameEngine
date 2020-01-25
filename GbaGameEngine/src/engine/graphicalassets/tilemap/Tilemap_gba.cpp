#include "Tilemap.h"
#include "TilemapSet.h"

Tilemap::Tilemap()
{

}

GBA::Background::ControlRegister::Size Tilemap::GetSize() const
{
	return GBA::Background::GetRegSizeFromTileSize(m_sizeInTiles.x, m_sizeInTiles.y);
}

bool Tilemap::IsLoaded() const
{
	return GetMapScreenBaseBlockIndex() != GBA::INVALID_SBB_ID && m_tilemapSet->IsLoaded();
}
