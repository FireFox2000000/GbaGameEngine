#include "SpriteManager.h"
#include "engine/graphicalassets/sprite/Sprite.h"
#include "engine/graphicalassets/sprite/SpriteAtlus.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/tiles/GBATileBank.h"

SpriteManager::SpriteManager()
	: m_paletteRefTracker(0)
	, m_tileRefTracker(Free)
{
}

SpriteManager::~SpriteManager()
{
}

tTileId SpriteManager::FindNextFreeTileSpace(u8 tileCount) const
{
	tTileId tileIndex = tileCount;		// We start from offset of a whole tile because tile 0 is reserved for disabled tiles

	while (tTileId(tileIndex + tileCount) < m_tileRefTracker.Count())
	{
		bool tileSpaceValid = true;

		for (u16 i = tileIndex; i < (tileIndex + tileCount); ++i)		// Check that the space for the tile actually exists
		{
			if (m_tileRefTracker[i] != Free)
			{
				tileSpaceValid = false;
				break;
			}
		}

		if (tileSpaceValid)
			return tileIndex;
		else
			tileIndex += tileCount;	// Align to the size to prevent fragmentation
	}

	return INVALID_TILE_ID;
}

void SpriteManager::Load(Sprite& out_sprite)
{
	using namespace GBA;

	if (out_sprite.IsLoaded())
		return;

	// Load palette
	SpriteAtlus* atlus = out_sprite.EditAtlus();
	tPaletteIndex paletteId = 0;

	if (!out_sprite.m_atlus->IsPaletteLoaded())
	{
		while (paletteId < m_paletteRefTracker.Count() && m_paletteRefTracker[paletteId] > 0)
			++paletteId;

		if (paletteId >= m_paletteRefTracker.Count())
			return;		// Out of palette space!

		ColourPalette16 palette(0);
			
		for (u32 i = 0; i < atlus->m_paletteLength; ++i)
		{
			palette[i] = atlus->m_palette[i];
		}
		PaletteBank::LoadSpritePalette(paletteId, palette);
		atlus->m_paletteIndex = paletteId;
	}
	else
	{
		paletteId = out_sprite.GetPaletteIndex();
	}

	++m_paletteRefTracker[paletteId];

	// Set tiles
	const u8 bitsPerByte = 8;
	u8 bitsPerPixel = 4;	// Todo, don't hardcode
	u8 pixelsPerByte = bitsPerByte / bitsPerPixel;
	u16 totalBytes = out_sprite.m_pixelMapDataLength * sizeof(out_sprite.m_pixelMapDataLength);
	u16 totalPixels = totalBytes * pixelsPerByte;
	u8 tileCount = totalPixels / Tile::PIXELS_PER_TILE;
	
	tTileId tileIndex = FindNextFreeTileSpace(tileCount);
	if (tileIndex != INVALID_TILE_ID)
	{
		TileBlockGroups tileBlockGroup = SpriteLower;
		TileBank::LoadTiles(out_sprite.m_pixelMapData, out_sprite.m_pixelMapDataLength, tileBlockGroup, tileIndex);	// Todo, use function that doesn't specify tile block group

		m_tileRefTracker[tileIndex] = Used;
		for (int i = tileIndex + 1; i < tileIndex + tileCount; ++i)
		{
			m_tileRefTracker[i] = Continue;
		}

		// Set sprite attributes
		out_sprite.m_tileIndex = tileIndex;
	}
	else
	{
		// Undo the palette we just loaded
		--m_paletteRefTracker[paletteId];
	}
}

void SpriteManager::Unload(Sprite * sprite)
{
	if (!sprite->IsLoaded())
		return;

	// Remove tile references
	tTileId index = sprite->m_tileIndex;
	m_tileRefTracker[index++] = Free;

	while (index < m_tileRefTracker.Count() && m_tileRefTracker[index] == Continue)
	{
		m_tileRefTracker[index++] = Free;
	}
	sprite->m_tileIndex = INVALID_TILE_ID;

	// Decrease palette references
	--m_paletteRefTracker[sprite->GetPaletteIndex()];
	if (m_paletteRefTracker[sprite->GetPaletteIndex()] <= 0)
	{
		// Free it so it can be reused
		SpriteAtlus* atlus = sprite->m_atlus;
		atlus->m_paletteIndex = INVALID_PALETTE_INDEX;
	}
}

void SpriteManager::Clear()
{
	for (u32 i = 0; i < m_paletteRefTracker.Count(); ++i)
	{
		m_paletteRefTracker[i] = 0;
	}
}
