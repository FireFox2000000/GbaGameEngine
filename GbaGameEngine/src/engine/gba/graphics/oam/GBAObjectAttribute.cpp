#include "GBAObjectAttribute.h"
#include "engine/algorithm/Compression.h"

#define CLEARED 0x0

constexpr u32 A0_YCOORD_BITINDEX = 0x0;
constexpr u32 A0_YCOORD_BITLENGTH = 0x8;

constexpr u32 A0_OBJMODE_BITINDEX = 0x8;
constexpr u32 A0_OBJMODE_BITLENGTH = 0x2;

constexpr u32 A0_GFXMODE_BITINDEX = 0xA;
constexpr u32 A0_GFXMODE_BITLENGTH = 0x2;

constexpr u32 A0_MOSAIC_BITINDEX = 0xC;
constexpr u32 A0_MOSAIC_BITLENGTH = 0x1;

constexpr u32 A0_COLOURMODE_BITINDEX = 0xD;
constexpr u32 A0_COLOURMODE_BITLENGTH = 0x1;

constexpr u32 A0_SHAPE_BITINDEX = 0xE;
constexpr u32 A0_SHAPE_BITLENGTH = 0x2;

constexpr u32 A1_XCOORD_BITINDEX = 0x0;
constexpr u32 A1_XCOORD_BITLENGTH = 0x9;

constexpr u32 A1_AFFINEINDEX_BITINDEX = 0x9;
constexpr u32 A1_AFFINEINDEX_BITLENGTH = 0x5;

constexpr u32 A1_HORIZONTALFLIP_BITINDEX = 0xC;
constexpr u32 A1_HORIZONTALFLIP_BITLENGTH = 0x1;

constexpr u32 A1_VERTICALFLIP_BITINDEX = 0xD;
constexpr u32 A1_VERTICALFLIP_BITLENGTH = 0x1;

constexpr u32 A1_SIZEMODE_BITINDEX = 0xE;
constexpr u32 A1_SIZEMODE_BITLENGTH = 0x2;

constexpr u32 A2_TILEID_BITINDEX = 0x0;
constexpr u32 A2_TILEID_BITLENGTH = 0xA;

constexpr u32 A2_PRIORITY_BITINDEX = 0xA;
constexpr u32 A2_PRIORITY_BITLENGTH = 0x2;

constexpr u32 A2_PALETTEBANK_BITINDEX = 0xC;
constexpr u32 A2_PALETTEBANK_BITLENGTH = 0x4;

GBA::Gfx::ObjectAttribute::ObjectAttribute()
	: m_attributeZero(0)
	, m_attributeOne(0)
	, m_attributeTwo(0) 
{
}

void GBA::Gfx::ObjectAttribute::Reset() volatile
{
	m_attributeZero = m_attributeOne = m_attributeTwo = 0;
}

void GBA::Gfx::ObjectAttribute::SetYPosition(int value) volatile
{
	SafeBitFieldSet(m_attributeZero, value, A0_YCOORD_BITINDEX, A0_YCOORD_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetObjectMode(Attributes::ObjectMode objectMode) volatile
{
	SafeBitFieldSet(m_attributeZero, objectMode, A0_OBJMODE_BITINDEX, A0_OBJMODE_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetGfxMode(Attributes::GfxMode gfxMode) volatile
{
	SafeBitFieldSet(m_attributeZero, gfxMode, A0_GFXMODE_BITINDEX, A0_GFXMODE_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetColourMode(Attributes::ColourMode colourMode) volatile
{
	SafeBitFieldSet(m_attributeZero, colourMode, A0_COLOURMODE_BITINDEX, A0_COLOURMODE_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetShape(Attributes::Shape shape) volatile
{
	SafeBitFieldSet(m_attributeZero, shape, A0_SHAPE_BITINDEX, A0_SHAPE_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetMosaic(bool enabled) volatile
{
	SafeBitFieldSet(m_attributeZero, enabled, A0_MOSAIC_BITINDEX, A0_MOSAIC_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetXPosition(int value) volatile
{
	SafeBitFieldSet(m_attributeOne, value, A1_XCOORD_BITINDEX, A1_XCOORD_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetSizeMode(Attributes::SizeMode sizeMode) volatile
{
	SafeBitFieldSet(m_attributeOne, sizeMode, A1_SIZEMODE_BITINDEX, A1_SIZEMODE_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetFlippedHorizontal() volatile
{
	SafeBitFieldSet(m_attributeOne, true, A1_HORIZONTALFLIP_BITINDEX, A1_HORIZONTALFLIP_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetFlippedVertical() volatile
{
	SafeBitFieldSet(m_attributeOne, true, A1_VERTICALFLIP_BITINDEX, A1_VERTICALFLIP_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::ClearFlippedHorizontal() volatile
{
	SafeBitFieldSet(m_attributeOne, false, A1_HORIZONTALFLIP_BITINDEX, A1_HORIZONTALFLIP_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::ClearFlippedVertical() volatile
{
	SafeBitFieldSet(m_attributeOne, false, A1_VERTICALFLIP_BITINDEX, A1_VERTICALFLIP_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetAffineIndex(u8 index) volatile
{
	SafeBitFieldSet(m_attributeOne, index, A1_AFFINEINDEX_BITINDEX, A1_AFFINEINDEX_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetPriority(DrawPriority layerNum) volatile
{
	SafeBitFieldSet(m_attributeTwo, layerNum, A2_PRIORITY_BITINDEX, A2_PRIORITY_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetTileIndex(u32 index) volatile
{
	SafeBitFieldSet(m_attributeTwo, index, A2_TILEID_BITINDEX, A2_TILEID_BITLENGTH);
}

void GBA::Gfx::ObjectAttribute::SetPaletteIndex(u8 index) volatile
{
	SafeBitFieldSet(m_attributeTwo, index, A2_PALETTEBANK_BITINDEX, A2_PALETTEBANK_BITLENGTH);
}

