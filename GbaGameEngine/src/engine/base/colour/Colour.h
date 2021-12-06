#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/math/Math.h"

typedef u16 rgb16;

class Colour
{
	static u8 ScaleToMaxRgb16(u8 colour);

public:
	// Values must be between 0 and 31
	static rgb16 RGB16(u8 r, u8 g, u8 b);

	u8 r, g, b, a;

	Colour();
	Colour(u8 r, u8 g, u8 b, u8 a);
	Colour(u8 r, u8 g, u8 b);
	~Colour();

	rgb16 RGB16() const { return Colour::RGB16(ScaleToMaxRgb16(r), ScaleToMaxRgb16(g), ScaleToMaxRgb16(b)); }

	const static Colour White;
	const static Colour Black;
	const static Colour Red;
	const static Colour Green;
	const static Colour Blue;
};
