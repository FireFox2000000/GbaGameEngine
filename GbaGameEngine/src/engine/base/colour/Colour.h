#ifndef PRAGMA_ONCE_ENGINE_BASE_COLOUR_COLOUR_H
#define PRAGMA_ONCE_ENGINE_BASE_COLOUR_COLOUR_H

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/math/Math.h"

typedef u16 rgb16;

class Colour
{
	static const u8 RGB16_MAX = 31;
	static const u8 RGB_MAX = 255;

	inline static u8 ScaleToMaxRgb16(u8 colour) { return u8(ROUND(float(colour) / RGB_MAX * RGB16_MAX)); }

public:
	inline static rgb16 RGB16(u8 r, u8 g, u8 b) { return (MIN(r, RGB16_MAX) + (MIN(g, RGB16_MAX) << 5) + (MIN(b, RGB16_MAX) << 10)); }		// Values must be between 0 and 31

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

#endif
