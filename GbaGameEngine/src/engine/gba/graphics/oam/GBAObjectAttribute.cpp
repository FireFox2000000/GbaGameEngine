#include "GBAObjectAttribute.h"
#define CLEARED 0x0

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

void GBA::Gfx::ObjectAttribute::SetObjectMode(Attributes::ObjectMode objectMode) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)objectMode, sc_A0_OBJMODE_BITINDEX), ObjMode);
}

void GBA::Gfx::ObjectAttribute::SetGfxMode(Attributes::GfxMode gfxMode) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)gfxMode, sc_A0_GFXMODE_BITINDEX), GfxMode);
}

void GBA::Gfx::ObjectAttribute::SetColourMode(Attributes::ColourMode colourMode) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)colourMode, sc_A0_COLOURMODE_BITINDEX), ColourMode);
}

void GBA::Gfx::ObjectAttribute::SetShape(Attributes::Shape shape) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)shape, sc_A0_SHAPE_BITINDEX), Shape);
}

void GBA::Gfx::ObjectAttribute::SetSizeMode(Attributes::SizeMode sizeMode) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)sizeMode, sc_A1_SIZEMODE_BITINDEX), SizeMode);
}

void GBA::Gfx::ObjectAttribute::SetPriority(DrawPriority layerNum) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)layerNum, sc_A2_PRIORITY_BITINDEX), Priority);
}

void GBA::Gfx::ObjectAttribute::SetTileIndex(u32 index) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)index, sc_A2_TILEID_BITINDEX), BaseTileIndex);
}

void GBA::Gfx::ObjectAttribute::SetPaletteIndex(u8 index) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)index, sc_A2_PALETTEBANK_BITINDEX), PaletteBank);
}

void GBA::Gfx::ObjectAttribute::SetAffineIndex(u8 index) volatile
{
	SetAttribute(SHIFTED_BITMASK((int)index, sc_A1_AFFINEINDEX_BITINDEX), AffineIndex);
}

void GBA::Gfx::ObjectAttribute::SetMosaic(bool enabled) volatile
{
	SetAttribute(enabled ? Mosaic : CLEARED, Mosaic);
}

void GBA::Gfx::ObjectAttribute::SetFlippedHorizontal() volatile
{
	SetAttribute(HorizontalFlip, HorizontalFlip);
}

void GBA::Gfx::ObjectAttribute::SetFlippedVertical() volatile
{
	SetAttribute(VerticalFlip, VerticalFlip);
}

void GBA::Gfx::ObjectAttribute::ClearFlippedHorizontal() volatile
{
	SetAttribute(0, HorizontalFlip);
}

void GBA::Gfx::ObjectAttribute::ClearFlippedVertical() volatile
{
	SetAttribute(0, VerticalFlip);
}
