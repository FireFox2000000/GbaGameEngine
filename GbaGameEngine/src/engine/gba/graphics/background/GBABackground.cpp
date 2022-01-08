#include "GBABackground.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/algorithm/Compression.h"

namespace GBA
{
	namespace Gfx
	{
		Background::ControlRegister::ColourMode Background::GetColourModeFromCompression(u32 compressionFlags)
		{
			return Compression::GetBitPackedSrcBpp(compressionFlags) > 4 ? Background::ControlRegister::ColourMode::EightBitsPerPixel : Background::ControlRegister::ColourMode::FourBitsPerPixel;
		}

		void Background::ControlRegister::SetPriority(DrawPriority priority)
		{
			m_priority = priority;
		}

		void Background::ControlRegister::SetCharacterBaseBlock(tCharacterBaseBlock blockId)
		{
			m_characterBaseBlock = blockId;
		}

		void Background::ControlRegister::SetMosaic(bool enabled)
		{
			m_mosaic = enabled;
		}

		void Background::ControlRegister::SetColourMode(ColourMode colourMode)
		{
			m_colourMode = colourMode;
		}

		void Background::ControlRegister::SetScreenBaseBlock(tScreenBaseBlockIndex blockId)
		{
			m_screenBaseBlock = blockId;
		}

		void Background::ControlRegister::SetAffineWrapping(bool enabled)
		{
			m_affineWrapping = enabled;
		}

		void Background::ControlRegister::SetSize(Size size)
		{
			m_backgroundSize = size;
		}

		typedef Vector2<u8> tTileSize;
		const u8 c_SIZEMAP_COUNT = Background::ControlRegister::REGSize_Count;
		const tTileSize c_SIZEMAP[c_SIZEMAP_COUNT] = {
			tTileSize(32, 32),	tTileSize(64, 32),	tTileSize(32, 64),	tTileSize(64, 64),
		};

		Background::ControlRegister::Size Background::GetRegSizeFromTileSize(u8 width, u8 height)
		{
			tTileSize tileSize(width, height);

			for (u8 i = 0; i < c_SIZEMAP_COUNT; ++i)
			{
				if (c_SIZEMAP[i] == tileSize)
				{
					return ControlRegister::Size(i);
				}
			}

			DEBUG_ERROR("Invalid size, cannot determine GBA Control Register size");

			return ControlRegister::Size::REGSize_Count;
		}
	}
}
