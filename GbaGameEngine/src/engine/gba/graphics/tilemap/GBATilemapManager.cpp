#include "GBATilemapManager.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVramAllocator.h"
#include "engine/gba/graphics/tilemap/GBATilemap.h"
#include "engine/gba/graphics/tilemap/GBATilemapSet.h"
#include "engine/gba/registers/display/GBABackgroundAllocator.h"
#include "gbatek/Backgrounds.h"

using namespace GBA::Gfx;

const Vector2<u8> TilemapManager::VARIABLE_TILEMAP_SIZE = Vector2<u8>(32, 32);

namespace
{
	using tTileSize = Vector2<u8>;
	const Array<tTileSize, 4> c_SIZEMAP = {
		tTileSize(32, 32),	tTileSize(64, 32),	tTileSize(32, 64),	tTileSize(64, 64),
	};

	GBATEK::BackgroundSize GetRegSizeFromTileSize(u8 width, u8 height)
	{
		tTileSize tileSize(width, height);

		for (u8 i = 0; i < c_SIZEMAP.Count(); ++i)
		{
			if (c_SIZEMAP[i] == tileSize)
			{
				return GBATEK::BackgroundSize(i);
			}
		}

		DEBUG_ERROR("Invalid size, cannot determine GBA Control Register size");

		return GBATEK::BackgroundSize(-1);
	}

	GBATEK::BackgroundSize GetTilemapSize(const Tilemap& tilemap)
	{
		return GetRegSizeFromTileSize(tilemap.GetFile().m_sizeInTiles.x, tilemap.GetFile().m_sizeInTiles.y);
	}
}

TilemapManager::TilemapManager()
	: m_tilesetRefCounter(0)
{
}

TilemapManager::~TilemapManager()
{
}

void TilemapManager::LoadStaticMap(Tilemap & out_tilemap)
{
	Load(out_tilemap, out_tilemap.m_file.m_tileMapDataLength, GetTilemapSize(out_tilemap), false, true);
}

void TilemapManager::LoadDynamicMap(Tilemap & out_tilemap)
{
	Load(out_tilemap, VARIABLE_TILEMAP_SIZE.x * VARIABLE_TILEMAP_SIZE.y, GBATEK::BackgroundSize::Regular32x32, false, false);
}

void TilemapManager::LoadTilemap(Tilemap & out_tilemap)
{
	if (out_tilemap.IsDynamicallyRendered())
	{
		LoadDynamicMap(out_tilemap);
	}
	else
	{
		LoadStaticMap(out_tilemap);
	}
}

void TilemapManager::LoadPalette(TilemapSet* tilemapSet)
{
	tPaletteIndex paletteIndex = tilemapSet->m_file.m_paletteBankIndex;
	tilemapSet->m_renderData.m_paletteIndex = paletteIndex;
	GBA::Gfx::PaletteBank::LoadBackgroundPalette(paletteIndex, tilemapSet->m_file.m_palette, tilemapSet->m_file.m_paletteLength);
}

void TilemapManager::LoadTileset(TilemapSet* tilemapSet)
{
	using namespace GBA;

	auto& vram = VramAllocator::GetInstance();
	tilemapSet->m_renderData.m_tileSetCharacterBaseBlock = vram.AllocBackgroundTileSetMem(tilemapSet->m_file.m_tilesetLength);

	if (tilemapSet->m_renderData.m_tileSetCharacterBaseBlock != TileBlockGroups::BlockGroupCount)	// Check that the alloc was actually successfull
	{
		vram.LoadBackgroundTileSetMem(tilemapSet->m_file.m_tileset, tilemapSet->m_file.m_tilesetLength, tilemapSet->m_renderData.m_tileSetCharacterBaseBlock);
	}
}

void TilemapManager::Load(Tilemap & out_tilemap, u32 tilesToAlloc, GBATEK::BackgroundSize size, bool isAffine, bool copyMapDirectlyToMemory)
{
	using namespace GBA;

	if (isAffine)
	{
		DEBUG_ERROR("Affine backgrounds not supported");
		return;
	}

	TilemapSet* tilemapSet = out_tilemap.EditTilemapSet();

	bool addRefCount = false;

	// Load tileset
	if (!tilemapSet->IsLoaded())
	{
		// TODO, need better system to handle multiple tilemap sets being loaded. Explicitly fail to load multiple in the mean time
		// Tilemap sets may use a predefined palette index, rather than trying to define it explicitly

		for (u8 refCount : m_tilesetRefCounter)
		{
			if (refCount > 0)
			{
				DEBUG_ERROR("Unable to load tilemap set. System not configured to handle multiple tilemaps from different sets.");
				return;
			}
		}

		LoadPalette(tilemapSet);
		LoadTileset(tilemapSet);

		addRefCount = true;
	}

	// Load local map data
	if (!out_tilemap.IsLoaded())
	{
		out_tilemap.m_renderData.m_mapSbbIndex = VramAllocator::GetInstance().AllocBackgroundTileMapMem(tilesToAlloc);

		if (copyMapDirectlyToMemory)
		{
			VramAllocator::GetInstance().LoadBackgroundTileMapMem(out_tilemap.m_file.m_tileMapData, out_tilemap.m_file.m_tileMapDataLength, out_tilemap.m_renderData.m_mapSbbIndex);
		}

		// Assign background slot
		{
			out_tilemap.m_renderData.m_backgroundSlotId = GBA::BackgroundAllocator::ReserveBackground();

			DEBUG_ASSERTMSG(out_tilemap.m_renderData.m_backgroundSlotId != GBA::BackgroundAllocator::INVALID_BACKGROUND, "Failed to assign background slot id, out of backgrounds");
		}

		addRefCount = true;
	}

	if (addRefCount && tilemapSet->m_renderData.m_tileSetCharacterBaseBlock != TilemapSet::INVALID_TILESET_CBB)
	{
		++m_tilesetRefCounter[tilemapSet->m_renderData.m_tileSetCharacterBaseBlock];
	}

	// Finally put our tilemap into the registers
	{
		GBATEK::BackgroundColourMode colourMode = tilemapSet->m_file.m_backgroundColourMode;
		auto& controlRegister = (*GBATEK::ioRegisterBackgroundControls)[out_tilemap.GetAssignedBackgroundSlot()];
		controlRegister.colourMode = colourMode;
		controlRegister.vramCharacterBaseBlockIndex = tilemapSet->GetTileSetCharacterBaseBlock();
		controlRegister.vramScreenBaseBlockIndex = out_tilemap.GetMapScreenBaseBlockIndex();
		controlRegister.size = size;
		controlRegister.mosaic = false;
	}
}

void TilemapManager::Unload(Tilemap * tilemap)
{
	using namespace GBA;

	if (tilemap->IsLoaded())
	{
		// TODO, maybe turn off the bg first?

		// Free map memory
		{
			VramAllocator::GetInstance().FreeBackgroundTileMapMem(tilemap->m_renderData.m_mapSbbIndex);
			tilemap->m_renderData.m_mapSbbIndex = VramAllocator::INVALID_SBB_ID;
		}

		// Free background slot
		{
			GBA::BackgroundAllocator::FreeBackground(tilemap->m_renderData.m_backgroundSlotId);
			tilemap->m_renderData.m_backgroundSlotId = GBA::BackgroundAllocator::INVALID_BACKGROUND;
		}

		TilemapSet* tilemapSet = tilemap->EditTilemapSet();
		--m_tilesetRefCounter[tilemapSet->m_renderData.m_tileSetCharacterBaseBlock];

		// Free tilemap set memory
		if (m_tilesetRefCounter[tilemapSet->m_renderData.m_tileSetCharacterBaseBlock] <= 0)
		{
			VramAllocator::GetInstance().FreeBackgroundTileSetMem(tilemapSet->m_renderData.m_tileSetCharacterBaseBlock);
			tilemapSet->m_renderData.m_tileSetCharacterBaseBlock = TilemapSet::INVALID_TILESET_CBB;
		}
	}
}

void TilemapManager::Clear()
{
	m_tilesetRefCounter.SetAllTo(0);
}
