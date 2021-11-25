#include "GBABackground.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/algorithm/Compression.h"

#define CLEARED 0x0

static const u32 sc_PRIORITY_BITINDEX = 0x0;
static const u32 sc_CBB_BITINDEX = 0x2;
static const u32 sc_MOSAIC_BITINDEX = 0x6;
static const u32 sc_COLOURMODE_BITINDEX = 0x7;
static const u32 sc_SBB_BITINDEX = 0x8;
static const u32 sc_AFFWRAPPING_BITINDEX = 0xD;
static const u32 sc_SIZE_BITINDEX = 0xE;

enum Masks
{
	Priorty = BITS_INDEXED_U32(2, sc_PRIORITY_BITINDEX),
	CharacterBaseBlock = BITS_INDEXED_U32(2, sc_CBB_BITINDEX),
	Mosaic = BITS_INDEXED_U32(2, sc_MOSAIC_BITINDEX),
	ColourMode = BITS_INDEXED_U32(1, sc_COLOURMODE_BITINDEX),
	ScreenBaseBlock = BITS_INDEXED_U32(5, sc_SBB_BITINDEX),
	AffineWrapping = BITS_INDEXED_U32(1, sc_AFFWRAPPING_BITINDEX),
	Size = BITS_INDEXED_U32(2, sc_SIZE_BITINDEX),

	Count = 7
};

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
			SetControlRegister(SHIFTED_BITMASK(priority, sc_PRIORITY_BITINDEX), Masks::Priorty);
		}

		void Background::ControlRegister::SetCharacterBaseBlock(tCharacterBaseBlock blockId)
		{
			SetControlRegister(SHIFTED_BITMASK(blockId, sc_CBB_BITINDEX), Masks::CharacterBaseBlock);
		}

		void Background::ControlRegister::SetMosaic(bool enabled)
		{
			SetControlRegister(enabled ? Masks::Mosaic : CLEARED, Masks::Mosaic);
		}

		void Background::ControlRegister::SetColourMode(ColourMode colourMode)
		{
			SetControlRegister(SHIFTED_BITMASK(colourMode, sc_COLOURMODE_BITINDEX), Masks::ColourMode);
		}

		void Background::ControlRegister::SetScreenBaseBlock(tScreenBaseBlockIndex blockId)
		{
			SetControlRegister(SHIFTED_BITMASK(blockId, sc_SBB_BITINDEX), Masks::ScreenBaseBlock);
		}

		void Background::ControlRegister::SetAffineWrapping(bool enabled)
		{
			SetControlRegister(enabled ? Masks::AffineWrapping : CLEARED, Masks::AffineWrapping);
		}

		void Background::ControlRegister::SetSize(Size size)
		{
			SetControlRegister(SHIFTED_BITMASK(size, sc_SIZE_BITINDEX), Masks::Size);
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
