#pragma once
#include "../Typedefs.h"
#include "../Macros.h"

typedef u16 rgb16;

class Colour
{
	static const u8 RGB16_MAX = 31;
	static const u8 RGB_MAX = 255;

	inline static float ScaleRgb256(float colourValue) { return colourValue / float(RGB_MAX); }	// Returns scale between 0 and 1

public:
	inline static rgb16 RGB16(u8 r, u8 g, u8 b) { return (MIN(r, RGB16_MAX) + (MIN(g, RGB16_MAX) << 5) + (MIN(b, RGB16_MAX) << 10)); }		// Values must be between 0 and 31

	float r, g, b, a;

	Colour();
	Colour(float r, float g, float b);
	Colour(float r, float g, float b, float a);
	~Colour();

	rgb16 RGB16() const { return Colour::RGB16(ROUND(r * RGB16_MAX), ROUND(g * RGB16_MAX), ROUND(b * RGB16_MAX)); }

	static Colour White() { return Colour(1, 1, 1); }
	static Colour Black() { return Colour(0, 0, 0); }
	static Colour Red() { return Colour(1, 0, 0); }
	static Colour Green() { return Colour(0, 1, 0); }
	static Colour Blue() { return Colour(0, 0, 1); }

	static Colour ColourU8(u8 r, u8 g, u8 b, u8 a) { return Colour(ScaleRgb256(r), ScaleRgb256(g), ScaleRgb256(b), ScaleRgb256(a)); }
	static Colour ColourU8(u8 r, u8 g, u8 b) { return Colour(ScaleRgb256(r), ScaleRgb256(g), ScaleRgb256(b)); }
};
