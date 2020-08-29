#include "TilemapManager.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVram.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"
#include "engine/graphicalassets/tilemap/Tilemap.h"
#include "engine/graphicalassets/tilemap/TilemapSet.h"

const Vector2<u8> TilemapManager::VARIABLE_TILEMAP_SIZE = Vector2<u8>(32, 32);

TilemapManager::TilemapManager()
	: m_tilesetRefCounter(0)
{
}

TilemapManager::~TilemapManager()
{
}

void TilemapManager::LoadStaticMap(Tilemap & out_tilemap)
{
	Load(out_tilemap, out_tilemap.m_tileMapDataLength, out_tilemap.GetSize(), false, true);
}

void TilemapManager::LoadDynamicMap(Tilemap & out_tilemap)
{
	Load(out_tilemap, VARIABLE_TILEMAP_SIZE.x * VARIABLE_TILEMAP_SIZE.y, GBA::Gfx::Background::ControlRegister::Size::REG_32x32, false, false);
}

void TilemapManager::Load(Tilemap & out_tilemap, u32 tilesToAlloc, GBA::Gfx::Background::ControlRegister::Size size, bool isAffine, bool copyMapDirectlyToMemory)
{
	using namespace GBA;

	if (isAffine)
	{
		DEBUG_ASSERTMSG(false, "Affine backgrounds not supported");
		return;
	}

	TilemapSet* tilemapSet = out_tilemap.EditTilemapSet();

	bool addRefCount = false;

	// Load tileset
	if (!tilemapSet->IsLoaded())
	{
		// TODO, need better system to handle multiple tilemap sets being loaded. Explicitly fail to load multiple in the mean time

		for (u8 refCount : m_tilesetRefCounter)
		{
			if (refCount > 0)
			{
				DEBUG_ASSERTMSG(false, "Unable to load tilemap set. System not configured to handle multiple tilemaps from different sets.");
				return;
			}
		}

		tilemapSet->m_paletteIndex = 0;
		Gfx::PaletteBank::LoadBackgroundPalette(tilemapSet->m_palette, tilemapSet->m_paletteLength);

		auto& vram = Vram::GetInstance();
		tilemapSet->m_tileSetCharacterBaseBlock = vram.AllocBackgroundTileSetMem(tilemapSet->m_tilesetLength);

		if (tilemapSet->m_tileSetCharacterBaseBlock != TileBlockGroups::BlockGroupCount)	// Check that the alloc was actually successfull
		{
			vram.LoadBackgroundTileSetMem(tilemapSet->m_tileset, tilemapSet->m_tilesetLength, tilemapSet->m_tileSetCharacterBaseBlock);
		}

		addRefCount = true;
	}

	// Load local map data
	if (!out_tilemap.IsLoaded())
	{
		out_tilemap.m_mapSbbIndex = Vram::GetInstance().AllocBackgroundTileMapMem(tilesToAlloc);

		if (copyMapDirectlyToMemory)
		{
			Vram::GetInstance().LoadBackgroundTileMapMem(out_tilemap.m_tileMapData, out_tilemap.m_tileMapDataLength, out_tilemap.m_mapSbbIndex);
		}

		// Assign background slot
		{
			out_tilemap.m_backgroundSlotId = GBA::BackgroundControl::ReserveBackground();

			DEBUG_ASSERTMSG(out_tilemap.m_backgroundSlotId < GBA::BackgroundControl::Count, "Failed to assign background slot id, out of backgrounds");
		}

		addRefCount = true;
	}

	if (addRefCount && tilemapSet->m_tileSetCharacterBaseBlock != TilemapSet::INVALID_TILESET_CBB)
	{
		++m_tilesetRefCounter[tilemapSet->m_tileSetCharacterBaseBlock];
	}

	// Finally put our tilemap into the registers
	{
		GBA::Gfx::Background::ControlRegister::ColourMode colourMode = GBA::Gfx::Background::GetColourModeFromCompression(tilemapSet->m_tileSetDataCompressionFlags);
		auto& controlRegister = BackgroundControl::GetBgControlRegister(out_tilemap.GetAssignedBackgroundSlot());
		controlRegister.SetColourMode(colourMode);
		controlRegister.SetCharacterBaseBlock(tilemapSet->GetTileSetCharacterBaseBlock());
		controlRegister.SetScreenBaseBlock(out_tilemap.GetMapScreenBaseBlockIndex());
		controlRegister.SetSize(size);
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
			Vram::GetInstance().FreeBackgroundTileMapMem(tilemap->m_mapSbbIndex);
			tilemap->m_mapSbbIndex = INVALID_SBB_ID;
		}

		// Free background slot
		{
			GBA::BackgroundControl::FreeBackground(tilemap->m_backgroundSlotId);
			tilemap->m_backgroundSlotId = GBA::BackgroundControl::Count;
		}

		TilemapSet* tilemapSet = tilemap->EditTilemapSet();
		--m_tilesetRefCounter[tilemapSet->m_tileSetCharacterBaseBlock];

		// Free tilemap set memory
		if (m_tilesetRefCounter[tilemapSet->m_tileSetCharacterBaseBlock] <= 0)
		{
			Vram::GetInstance().FreeBackgroundTileSetMem(tilemapSet->m_tileSetCharacterBaseBlock);
			tilemapSet->m_tileSetCharacterBaseBlock = TilemapSet::INVALID_TILESET_CBB;
		}
	}
}

void TilemapManager::Clear()
{
	m_tilesetRefCounter.SetAllTo(0);
}
