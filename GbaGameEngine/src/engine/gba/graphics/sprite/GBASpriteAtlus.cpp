#include "GBASpriteAtlus.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"

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

		SpriteAtlus::SpriteAtlus(
			const u8 paletteLength
			, const u16 * palette
			, const u32 compressionFlags
		) 
			: m_palette(nullptr)
			, m_paletteLength(paletteLength)
			, m_spriteDataCompressionFlags(compressionFlags)
			, m_spritesLLHead(nullptr)
		{
		}

		SpriteAtlus::SpriteAtlus(const SpriteAtlus & that)
		{
			*this = that;
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
	}
}
