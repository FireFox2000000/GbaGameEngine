#include "GBAObjectAttribute.h"
#include "engine/algorithm/Compression.h"
#include "engine/base/core/Memory.h"

GBA::Gfx::ObjectAttribute::ObjectAttribute()
{
	static_assert(sizeof(GBA::Gfx::ObjectAttribute) == sizeof(u16) * 4, "ObjectAttribute size must match GBA register");
	Reset();
}

void GBA::Gfx::ObjectAttribute::Reset() volatile
{
	data[0] = data[1] = data[2] = 0;
}

void GBA::Gfx::ObjectAttribute::SetYPosition(int value) volatile
{
	attributes.yCoord = value;
}

void GBA::Gfx::ObjectAttribute::SetObjectMode(Attributes::ObjectMode objectMode) volatile
{
	attributes.objMode = objectMode;
}

void GBA::Gfx::ObjectAttribute::SetGfxMode(Attributes::GfxMode gfxMode) volatile
{
	attributes.gfxMode = gfxMode;
}

void GBA::Gfx::ObjectAttribute::SetColourMode(Attributes::ColourMode colourMode) volatile
{
	attributes.colourMode = colourMode;
}

void GBA::Gfx::ObjectAttribute::SetShape(Attributes::Shape shape) volatile
{
	attributes.shape = shape;
}

void GBA::Gfx::ObjectAttribute::SetMosaic(bool enabled) volatile
{
	attributes.mosaic = enabled;
}

void GBA::Gfx::ObjectAttribute::SetXPosition(int value) volatile
{
	attributes.xCoord = value;
}

void GBA::Gfx::ObjectAttribute::SetSizeMode(Attributes::SizeMode sizeMode) volatile
{
	attributes.sizeMode = sizeMode;
}

void GBA::Gfx::ObjectAttribute::SetFlippedHorizontal() volatile
{
	attributes.horizontalFlip = true;
}

void GBA::Gfx::ObjectAttribute::SetFlippedVertical() volatile
{
	attributes.verticalFlip = true;
}

void GBA::Gfx::ObjectAttribute::ClearFlippedHorizontal() volatile
{
	attributes.horizontalFlip = false;
}

void GBA::Gfx::ObjectAttribute::ClearFlippedVertical() volatile
{
	attributes.verticalFlip = false;
}

void GBA::Gfx::ObjectAttribute::SetAffineIndex(u8 index) volatile
{
	affineModeAttr.affineIndex = index;
}

void GBA::Gfx::ObjectAttribute::SetPriority(DrawPriority layerNum) volatile
{
	attributes.priority = layerNum;
}

void GBA::Gfx::ObjectAttribute::SetTileIndex(u16 index) volatile
{
	attributes.tileId = index;
}

void GBA::Gfx::ObjectAttribute::SetPaletteIndex(u8 index) volatile
{
	attributes.paletteBank = index;
}

