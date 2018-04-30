#include "GBASpriteLoader.h"
#include "engine\assets\GBASprite.h"
#include "engine\gba\graphics\oam\GBAAttributeFunctions.h"

#include "game\blobdata\Shantae.h"

namespace GBA
{
	SpriteLoader::SpriteLoader()
		: m_paletteRefTracker(0)
	{
#define SPRITE_ATLUS_ENTRY(Namespace) \
		{																						\
			SpriteData* data = m_spriteData.AddNew();											\
			data->width = Namespace::width;														\
			data->height = Namespace::height;													\
			data->paletteLength = sizeof(Namespace::palette) / sizeof(*Namespace::palette);		\
			data->dataLength = sizeof(Namespace::data) / sizeof(*Namespace::data);				\
			data->palette = Namespace::palette;													\
			data->pixelMapData = Namespace::data;												\
			data->paletteIndex = -1;															\
		}

		SPRITE_ATLUS_LIST

#undef SPRITE_ATLUS_ENTRY
	}

	SpriteLoader::~SpriteLoader()
	{
	}

	Sprite * SpriteLoader::Load(SpriteAtlus::Enum spriteAtlus)
	{
		SpriteData& data = m_spriteData[spriteAtlus];
		{
			Sprite* sprite = m_spriteList.AddNew();

			// Load palette
			tPaletteBlockId paletteId = 0;
			if (data.paletteIndex < 0)
			{
				while (paletteId < m_paletteRefTracker.Count() && m_paletteRefTracker[paletteId] > 0)
					++paletteId;

				if (paletteId >= m_paletteRefTracker.Count())
					return NULL;		// Out of palette space!

				ColourPalette16 palette(0);
				for (u32 i = 0; i < data.paletteLength; ++i)
				{
					palette[i] = data.palette[i];
				}
				PaletteBank::LoadSpritePalette(paletteId, palette);
				data.paletteIndex = paletteId;
			}
			else
			{
				paletteId = tPaletteBlockId(data.paletteIndex);
			}

			++m_paletteRefTracker[paletteId];

			// Set tiles
			tTileId tileIndex = 4;	// TODO
			{
				TileBank::LoadTiles(data.pixelMapData, data.dataLength, SpriteLower, tileIndex);	// Todo, use function that doesn't specify tile block group
				//TileBank::LoadSpriteTiles(tileData, tileIndex);
			}

			// Set sprite attributes
			{
				Attributes::Shape shape;
				Attributes::SizeMode size;
				AttributeFunctions::GetSizeAttributesFromPixelSize(Vector2(data.width, data.height), shape, size);

				sprite->shape = shape;
				sprite->sizeMode = size;
				sprite->paletteId = paletteId;
				sprite->tileIndex = tileIndex;
				sprite->isLoaded = true;
			}

			return sprite;
		}
		return NULL;
	}

	void SpriteLoader::Dispose(Sprite * sprite)
	{
		--m_paletteRefTracker[sprite->paletteId];
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
}
