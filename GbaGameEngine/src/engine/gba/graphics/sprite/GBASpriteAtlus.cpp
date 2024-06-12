#include "GBASpriteAtlus.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"
#include "engine/io/filestream/CppFileReader.h"
#include "engine/base/core/stl/Pool.h"
#include "gbatek/Vram.h"

namespace GBA
{
	namespace Gfx
	{
		SpriteAtlus::SpriteAtlus()
			: m_palette(nullptr)
			, m_paletteLength(0)
			, m_spriteDataCompressionFlags(0)
			, m_spritesLLHead(nullptr)
		{
		}

		bool SpriteAtlus::IsPaletteLoaded()
		{ 
			return GetPaletteIndex() != INVALID_PALETTE_INDEX; 
		}

		tPaletteIndex SpriteAtlus::GetPaletteIndex()
		{ 
			return m_paletteIndex; 
		}

		u32 SpriteAtlus::GetSpriteDataCompressionFlags()
		{
			return m_spriteDataCompressionFlags;
		}

		SpriteNode* SpriteAtlus::GetHead() const
		{ 
			return m_spritesLLHead; 
		}

		Sprite* SpriteAtlus::GetSprite(int index)
		{
			int currentIndex = 0;
			SpriteNode* currentNode = m_spritesLLHead;

			while (currentIndex < index && currentNode != nullptr)
			{
				currentNode = currentNode->next;
				++currentIndex;
			}

			return currentNode ? &currentNode->sprite : nullptr;
		}

		const Sprite* SpriteAtlus::GetSprite(int index) const
		{
			int currentIndex = 0;
			SpriteNode* currentNode = m_spritesLLHead;

			while (currentIndex < index && currentNode != nullptr)
			{
				currentNode = currentNode->next;
				++currentIndex;
			}

			return currentNode ? &currentNode->sprite : nullptr;
		}

		SpriteAtlus* SpriteAtlus::CreateFromFile(const u32* file, IPool<SpriteAtlus>* spriteAtlusPool, IPool<SpriteNode>* spriteNodePool)
		{
			CppFileReader reader = CppFileReader(file);

			const u32 spriteCount = reader.Read<u32>();
			const u8 paletteLength = reader.Read<u8>();
			const u32 dataLength = reader.Read<u32>();
			const u32 compressionFlags = reader.Read<u32>();
			const GBA::ColourRGB16* palette = reader.ReadAddress<GBA::ColourRGB16>(paletteLength);
			const u8* widthMap = reader.ReadAddress<u8>(spriteCount);
			const u8* heightMap = reader.ReadAddress<u8>(spriteCount);
			const u32* offsets = reader.ReadAddress<u32>(spriteCount);
			const GBA::UPixelData* data = reader.ReadAddress<GBA::UPixelData>(dataLength);

			DEBUG_LOGFORMAT("Loaded sprite atlus of size %.2fkb", BYTES_TO_KB(dataLength * sizeof(u32)));

			SpriteAtlus* atlus = spriteAtlusPool->CreateNew();

			if (!atlus)
			{
				DEBUG_ERROR("Sprite atlus pool out of memory");
				return nullptr;
			}

			atlus->m_paletteLength = paletteLength;
			atlus->m_palette = palette;
			atlus->m_spriteDataCompressionFlags = compressionFlags;

			using namespace GBA::Gfx;
			SpriteNode* lastAddedNode = nullptr;

			for (u32 i = 0; i < spriteCount; ++i)
			{
				auto* node = spriteNodePool->CreateNew();

				if (!node)
				{
					DEBUG_ERROR("Sprite node pool out of memory");
					return nullptr;
				}

				if (!atlus->m_spritesLLHead)
				{
					atlus->m_spritesLLHead = node;
				}

				if (lastAddedNode)
				{
					lastAddedNode->next = node;
				}

				ObjectShape shape;
				ObjectSize sizeMode;
				AttributeFunctions::GetSizeAttributesFromPixelSize(GBAAttrFnVector2(widthMap[i], heightMap[i]), shape, sizeMode);

				node->sprite.m_objectShape = shape;
				node->sprite.m_objectSize = sizeMode;
				node->sprite.m_tileSize = AttributeFunctions::GetTileSize(shape, sizeMode);
				node->sprite.m_atlus = atlus;
				node->sprite.m_pixelMapData = data + offsets[i];

				if (i + 1 < spriteCount)
				{
					node->sprite.m_pixelMapDataLength = offsets[i + 1] - offsets[i];
				}
				else
				{
					node->sprite.m_pixelMapDataLength = dataLength - offsets[i];
				}

				lastAddedNode = node;
			}

			return atlus;
		}
	}
}
