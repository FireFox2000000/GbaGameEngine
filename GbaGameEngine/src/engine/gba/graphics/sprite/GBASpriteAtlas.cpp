#include "GBASpriteAtlas.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"
#include "engine/io/filestream/MemoryMappedFileStream.h"
#include "engine/base/core/stl/Pool.h"
#include "gbatek/Vram.h"

namespace GBA
{
	namespace Gfx
	{
		SpriteAtlas::SpriteAtlas()
			: m_spriteDataCompressionFlags(0)
			, m_spritesLLHead(nullptr)
		{
		}

		bool SpriteAtlas::IsPaletteLoaded()
		{ 
			return GetPaletteIndex() != INVALID_PALETTE_INDEX; 
		}

		tPaletteIndex SpriteAtlas::GetPaletteIndex()
		{ 
			return m_paletteIndex; 
		}

		u32 SpriteAtlas::GetSpriteDataCompressionFlags()
		{
			return m_spriteDataCompressionFlags;
		}

		SpriteNode* SpriteAtlas::GetHead() const
		{ 
			return m_spritesLLHead; 
		}

		Sprite* SpriteAtlas::GetSprite(int index)
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

		const Sprite* SpriteAtlas::GetSprite(int index) const
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

		SpriteAtlas* SpriteAtlas::CreateFromFile(MemoryMappedFileStream& istream, IPool<SpriteAtlas>* spriteAtlasPool, IPool<SpriteNode>* spriteNodePool)
		{
			const u32 spriteCount = istream.Read<u32>();
			const u8 paletteLength = istream.Read<u8>();
			const u32 dataLength = istream.Read<u32>();
			const u32 compressionFlags = istream.Read<u32>();
			Span<const GBATEK::ColourRGB16> palette = istream.Read<GBATEK::ColourRGB16>(paletteLength);
			Span<const u8> widthMap = istream.Read<u8>(spriteCount);
			Span<const u8> heightMap = istream.Read<u8>(spriteCount);
			Span<const u32> offsets = istream.Read<u32>(spriteCount);
			Span<const GBATEK::UPixelData> data = istream.Read<GBATEK::UPixelData>(dataLength);

			DEBUG_LOGFORMAT("Loaded sprite atlas of size %.2fkb", BYTES_TO_KB(dataLength * sizeof(u32)));

			SpriteAtlas* atlas = spriteAtlasPool->CreateNew();
			atlas->m_assetHash = std::bit_cast<int>(istream.GetFileLocation());

			if (!atlas)
			{
				DEBUG_ERROR("Sprite atlas pool out of memory");
				return nullptr;
			}

			atlas->m_palette = palette;
			atlas->m_spriteDataCompressionFlags = compressionFlags;

			using namespace GBA::Gfx;
			SpriteNode* lastAddedNode = nullptr;

			DEBUG_LOGFORMAT("Loading %d sprites from atlas", spriteCount);

			for (u32 i = 0; i < spriteCount; ++i)
			{
				auto* node = spriteNodePool->CreateNew();

				if (!node)
				{
					DEBUG_ERROR("Sprite node pool out of memory");
					return nullptr;
				}

				if (!atlas->m_spritesLLHead)
				{
					atlas->m_spritesLLHead = node;
				}

				if (lastAddedNode)
				{
					lastAddedNode->next = node;
				}

				GBATEK::ObjectShape shape;
				GBATEK::ObjectSize sizeMode;
				AttributeFunctions::GetSizeAttributesFromPixelSize(Vector2<u8>(widthMap[i], heightMap[i]), shape, sizeMode);

				node->sprite.m_objectShape = shape;
				node->sprite.m_objectSize = sizeMode;
				node->sprite.m_tileSize = AttributeFunctions::GetTileSize(shape, sizeMode);
				node->sprite.m_atlas = atlas;

				if (i + 1 < spriteCount)
				{
					node->sprite.m_pixelMapData = data.Slice(offsets[i], offsets[i + 1] - offsets[i]);
				}
				else
				{
					node->sprite.m_pixelMapData = data.Slice(offsets[i], dataLength - offsets[i]);
				}

				lastAddedNode = node;
			}

			return atlas;
		}
	}
}
