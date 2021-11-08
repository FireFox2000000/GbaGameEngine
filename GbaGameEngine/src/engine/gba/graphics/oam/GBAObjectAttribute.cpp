#include "GBAObjectAttribute.h"
#define CLEARED 0x0

void GBA::Gfx::ObjectAttribute::SetMosaic(bool enabled) volatile
{
	SetAttribute(enabled ? Mosaic : CLEARED, Mosaic);
}
