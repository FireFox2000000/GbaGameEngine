#include "GBABackground.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/algorithm/Compression.h"

#define CLEARED 0x0

static const u32 sc_PRIORITY_BITINDEX = 0x0;
static const u32 sc_CBB_BITINDEX = 0x2;
static const u32 sc_MOSAIC_BITINDEX = 0x6;
static const u32 sc_COLOURMODE_BITINDEX = 0x7;
static const u32 sc_SBB_BITINDEX = 0x8;
static const u32 sc_AFFWRAPPING_BITINDEX = 0x8;
static const u32 sc_SIZE_BITINDEX = 0xE;

enum Masks
{
	Priorty = BITS_INDEXED_U32(2, sc_PRIORITY_BITINDEX),// 0-1
	CharacterBaseBlock = BITS_INDEXED_U32(2, sc_CBB_BITINDEX),		// 8-9
	Mosaic = BITS_INDEXED_U32(2, sc_MOSAIC_BITINDEX),				// A-B
	ColourMode = BITS_INDEXED_U32(1, sc_COLOURMODE_BITINDEX),				// C
	ScreenBaseBlock = BITS_INDEXED_U32(5, sc_SBB_BITINDEX),				// D 	
	AffineWrapping = BITS_INDEXED_U32(2, sc_AFFWRAPPING_BITINDEX),				// E-F
	Size = BITS_INDEXED_U32(2, sc_SIZE_BITINDEX),				// E-F

	Count = 7
};

u16& GBA::Background::EditControlRegister()
{
	return *(reinterpret_cast<u16*>(REG_BGCNT) + m_index);
}

GBA::Background::ColourMode GBA::Background::GetColourModeFromCompression(u32 compressionFlags)
{
	return Compression::GetBitPackedSrcBpp(compressionFlags) > 4 ? Background::ColourMode::EightBitsPerPixel : Background::ColourMode::FourBitsPerPixel;
}

void GBA::Background::SetPriority(Priority priority)
{
	SetControlRegister(SHIFTED_BITMASK(priority, sc_PRIORITY_BITINDEX), Masks::Priorty);
}

void GBA::Background::SetCharacterBaseBlock(tCharacterBaseBlock blockId)
{
	SetControlRegister(SHIFTED_BITMASK(blockId, sc_CBB_BITINDEX), Masks::CharacterBaseBlock);
}

void GBA::Background::SetMosaic(bool enabled)
{
	SetControlRegister(enabled ? Masks::Mosaic : CLEARED, Masks::Mosaic);
}

void GBA::Background::SetColourMode(ColourMode colourMode)
{
	SetControlRegister(SHIFTED_BITMASK(colourMode, sc_COLOURMODE_BITINDEX), Masks::ColourMode);
}

void GBA::Background::SetScreenBaseBlock(tScreenBaseBlockIndex blockId)
{
	SetControlRegister(SHIFTED_BITMASK(blockId, sc_SBB_BITINDEX), Masks::ScreenBaseBlock);
}

void GBA::Background::SetAffineWrapping(bool enabled)
{
	SetControlRegister(enabled ? Masks::AffineWrapping : CLEARED, Masks::AffineWrapping);
}

void GBA::Background::SetSize(Size size)
{
	SetControlRegister(SHIFTED_BITMASK(size, sc_SIZE_BITINDEX), Masks::Size);
}

typedef Vector2<u8> tTileSize;
const u8 c_SIZEMAP_COUNT = GBA::Background::REGSize_Count;
const tTileSize c_SIZEMAP[c_SIZEMAP_COUNT] = {
	tTileSize(32, 32),	tTileSize(64, 32),	tTileSize(32, 64),	tTileSize(64, 64),
};

GBA::Background::Size GBA::Background::GetRegSizeFromTileSize(u8 width, u8 height)
{
	tTileSize tileSize(width, height);

	for (u8 i = 0; i < c_SIZEMAP_COUNT; ++i)
	{
		if (c_SIZEMAP[i] == tileSize)
		{
			return Size(i);
		}
	}

	return Size::REGSize_Count;
}
