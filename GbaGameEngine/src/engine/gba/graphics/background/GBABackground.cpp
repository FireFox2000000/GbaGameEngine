#include "GBABackground.h"
#include "engine/gba/registers/RegisterMap.h"

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
