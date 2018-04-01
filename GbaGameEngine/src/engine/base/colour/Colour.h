#pragma once
#include "../Typedefs.h"
#include "../Macros.h"

typedef u16 rgb16;

class Colour
{
	static const u8 RGB16_MAX = 31;
	static const u8 RGB_MAX = 255;

	inline static float ScaleRgb256(u8 colourValue) { return colourValue / float(RGB_MAX); }	// Returns scale between 0 and 1

public:
	inline static rgb16 RGB16(u8 r, u8 g, u8 b) { return (MIN(r, RGB16_MAX) + (MIN(g, RGB16_MAX) << 5) + (MIN(b, RGB16_MAX) << 10)); }		// Values must be between 0 and 31

	u8 r, g, b, a;

	Colour();
	Colour(u8 r, u8 g, u8 b);
	Colour(u8 r, u8 g, u8 b, u8 a);
	~Colour();

	rgb16 RGB16() const { return Colour::RGB16(ROUND(ScaleRgb256(r) * RGB16_MAX), ROUND(ScaleRgb256(g) * RGB16_MAX), ROUND(ScaleRgb256(b) * RGB16_MAX)); }

	static Colour White() { return Colour(0xFF, 0xFF, 0xFF); }
	static Colour Red() { return Colour(0xFF, 0x00, 0x00); }
	static Colour Green() { return Colour(0x00, 0xFF, 0x00); }
	static Colour Blue() { return Colour(0x00, 0x00, 0xFF); }
};