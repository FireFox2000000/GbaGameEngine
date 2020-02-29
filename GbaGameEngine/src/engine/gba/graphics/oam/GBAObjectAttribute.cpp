#include "GBAObjectAttribute.h"
#define CLEARED 0x0

void GBA::Gfx::ObjectAttribute::SetMosaic(bool enabled) volatile
{
	SetAttribute(enabled ? Mosaic : CLEARED, Mosaic);
}

void GBA::Gfx::ObjectAttribute::SetFlippedHorizontal(bool flipped) volatile
{
	SetAttribute(flipped ? HorizontalFlip : CLEARED, HorizontalFlip);
}

void GBA::Gfx::ObjectAttribute::SetFlippedVertical(bool flipped) volatile
{
	SetAttribute(flipped ? VerticalFlip : CLEARED, VerticalFlip);
}
