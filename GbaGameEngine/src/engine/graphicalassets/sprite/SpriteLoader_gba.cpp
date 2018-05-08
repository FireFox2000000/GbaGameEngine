#include "SpriteLoader.h"
#include "engine\graphicalassets\sprite\Sprite.h"
#include "engine\graphicalassets\sprite\SpriteAtlus.h"
#include "engine\gba\graphics\oam\GBAAttributeFunctions.h"
#include "engine\gba\graphics\tiles\GBAPaletteBank.h"
#include "engine\gba\graphics\tiles\GBATileBank.h"
#include "engine\graphicalassets\tile\Tile.h"

static u16 cumulativeTileIndex = 1;

SpriteLoader::SpriteLoader()
	: m_paletteRefTracker(0)
{
}

SpriteLoader::~SpriteLoader()
{
}

void SpriteLoader::Load(Sprite& out_sprite)
{
	using namespace GBA;

	// Load palette
	SpriteAtlus* atlus = out_sprite.EditAtlus();
	tPaletteIndex paletteId = 0;
	if (out_sprite.GetPaletteIndex() == INVALID_PALETTE_INDEX)
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
		atlus->m_isPaletteLoaded = true;
	}
	else
	{
		paletteId = out_sprite.GetPaletteIndex();
	}

	++m_paletteRefTracker[paletteId];

	// Set tiles
	tTileId tileIndex = cumulativeTileIndex;	// TODO
	{
		TileBank::LoadTiles(out_sprite.m_pixelMapData, out_sprite.m_pixelMapDataLength, SpriteLower, tileIndex);	// Todo, use function that doesn't specify tile block group
		cumulativeTileIndex += out_sprite.m_pixelMapDataLength * 4 / Tile::PIXELS_PER_TILE;
	}

	// Set sprite attributes
	{
		out_sprite.m_tileIndex = tileIndex;
		out_sprite.m_tilesLoaded = true;
	}
}

void SpriteLoader::Dispose(Sprite * sprite)
{
	--m_paletteRefTracker[sprite->GetPaletteIndex()];
	m_spriteList.Remove(sprite);
}

void SpriteLoader::Clear()
{
	for (u32 i = 0; i < m_paletteRefTracker.Count(); ++i)
	{
		m_paletteRefTracker[i] = 0;
	}

	m_spriteList.Clear();
}
