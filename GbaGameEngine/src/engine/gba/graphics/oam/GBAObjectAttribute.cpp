#include "GBAObjectAttribute.h"
#define CLEARED 0x0

void GBA::ObjectAttribute::SetMosaic(bool enabled) volatile
{
	SetAttribute(enabled ? Mosaic : CLEARED, Mosaic);
}

void GBA::ObjectAttribute::SetFlippedHorizontal(bool flipped) volatile
{
	SetAttribute(flipped ? HorizontalFlip : CLEARED, HorizontalFlip);
}

void GBA::ObjectAttribute::SetFlippedVertical(bool flipped) volatile
{
	SetAttribute(flipped ? VerticalFlip : CLEARED, VerticalFlip);
}
