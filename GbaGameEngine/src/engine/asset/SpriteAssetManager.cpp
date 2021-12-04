#include "SpriteAssetManager.h"
#include "engine/filestream/CppFileReader.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"

using SpriteAtlus = GBA::Gfx::SpriteAtlus;
using SpriteNode = GBA::Gfx::SpriteNode;

SpriteAtlus* SpriteAssetManagerHelper::CreateSpriteAtlusFromFile(const u32* file, IPool<SpriteAtlus>* spriteAtlusPool, IPool<SpriteNode>* spriteNodePool)
{
	CppFileReader reader = CppFileReader(file);

	const u32 spriteCount = reader.Read<u32>();
	const u8 paletteLength = reader.Read<u8>();
	const u32 dataLength = reader.Read<u32>();
	const u32 compressionFlags = reader.Read<u32>();
	const u16* palette = reader.ReadAddress<u16>(paletteLength);
	const u8* widthMap = reader.ReadAddress<u8>(spriteCount);
	const u8* heightMap = reader.ReadAddress<u8>(spriteCount);
	const u32* offsets = reader.ReadAddress<u32>(spriteCount);
	const u32* data = reader.ReadAddress<u32>(dataLength);

	DEBUG_LOGFORMAT("Loaded sprite atlus of size %.2fkb", BYTES_TO_KB(dataLength));

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

		Attributes::Shape shape;
		Attributes::SizeMode sizeMode;
		AttributeFunctions::GetSizeAttributesFromPixelSize(GBAAttrFnVector2(widthMap[i], heightMap[i]), shape, sizeMode);

		node->sprite.m_attributes = shape | (sizeMode << 2);
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
