#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/colour/Colour.h"

typedef u8 tPaletteIndex;
const static u8 INVALID_PALETTE_INDEX = 255;

typedef Array<GBA::ColourRGB16, 16> ColourPalette16;
typedef Array<GBA::ColourRGB16, 256> ColourPalette256;
