#include "GBATilemap.h"
#include "GBATilemapSet.h"

using namespace GBA::Gfx;

Tilemap::Tilemap()
{

}

Background::ControlRegister::Size Tilemap::GetSize() const
{
	return Background::GetRegSizeFromTileSize(m_file.m_sizeInTiles.x, m_file.m_sizeInTiles.y);
}

bool Tilemap::IsLoaded() const
{
	return GetMapScreenBaseBlockIndex() != GBA::INVALID_SBB_ID && m_tilemapSet->IsLoaded();
}
