#include "TilemapManager.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVram.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"
#include "engine/graphicalassets/tilemap/Tilemap.h"
#include "engine/graphicalassets/tilemap/TilemapSet.h"

TilemapManager::TilemapManager()
	: m_tilesetRefCounter(0)
{
}

TilemapManager::~TilemapManager()
{
}

void TilemapManager::Load(Tilemap & out_tilemap)
{
	using namespace GBA;

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

		ColourPalette256 colourPalette(0);
		for (u32 i = 0; i < tilemapSet->m_paletteLength; ++i)
		{
			colourPalette[i] = tilemapSet->m_palette[i];
		}
		tilemapSet->m_paletteIndex = 0;

		PaletteBank::LoadBackgroundPalette(colourPalette);

		Vram::GetInstance().AllocBackgroundTileSetMem(tilemapSet->m_tileset, tilemapSet->m_tilesetLength, tilemapSet->m_tileSetCharacterBaseBlock);
		addRefCount = true;
	}

	// Load local map data
	if (!out_tilemap.IsLoaded())
	{
		Vram::GetInstance().AllocBackgroundTileMapMem(out_tilemap.m_tileMapData, out_tilemap.m_tileMapDataLength, out_tilemap.m_mapSbbIndex);
		addRefCount = true;
	}

	if (addRefCount && tilemapSet->m_tileSetCharacterBaseBlock != TilemapSet::INVALID_TILESET_CBB)
	{
		++m_tilesetRefCounter[tilemapSet->m_tileSetCharacterBaseBlock];
	}
}

void TilemapManager::Unload(Tilemap * tilemap)
{
	using namespace GBA;

	if (tilemap->IsLoaded())
	{
		Vram::GetInstance().FreeBackgroundTileMapMem(tilemap->m_mapSbbIndex);
		tilemap->m_mapSbbIndex = INVALID_SBB_ID;

		TilemapSet* tilemapSet = tilemap->EditTilemapSet();
		--m_tilesetRefCounter[tilemapSet->m_tileSetCharacterBaseBlock];

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
