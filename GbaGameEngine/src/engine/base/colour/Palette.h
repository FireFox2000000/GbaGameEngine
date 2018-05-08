#ifndef PRAGMA_ONCE_ENGINE_BASE_COLOUR_PALETTE_H
#define PRAGMA_ONCE_ENGINE_BASE_COLOUR_PALETTE_H

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/colour/Colour.h"

typedef u8 tPaletteIndex;
const static u8 INVALID_PALETTE_INDEX = 0xFFFF;

typedef Array<rgb16, 16> ColourPalette16;
typedef Array<rgb16, 256> ColourPalette256;

#endif
