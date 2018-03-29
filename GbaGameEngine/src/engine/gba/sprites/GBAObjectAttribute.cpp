#include "GBAObjectAttribute.h"
#define CLEARED 0x0

void GBA::ObjectAttribute::SetMosaic(bool enabled)
{
	SetAttribute(enabled ? Mosaic : CLEARED, Mosaic);
}

void GBA::ObjectAttribute::SetFlippedHorizontal(bool flipped)
{
	SetAttribute(flipped ? HorizontalFlip : CLEARED, HorizontalFlip);
}

void GBA::ObjectAttribute::SetFlippedVertical(bool flipped)
{
	SetAttribute(flipped ? VerticalFlip : CLEARED, VerticalFlip);
}
