#include "Colour.h"

const u8 RGB16_MAX = 31;
const u8 RGB_MAX = 255;

const Colour Colour::White = Colour(RGB_MAX, RGB_MAX, RGB_MAX);
const Colour Colour::Black = Colour(0, 0, 0);
const Colour Colour::Red = Colour(RGB_MAX, 0, 0);
const Colour Colour::Green = Colour(0, RGB_MAX, 0);
const Colour Colour::Blue = Colour(0, 0, RGB_MAX);


u8 Colour::ScaleToMaxRgb16(u8 colour)
{
	return u8(ROUND(float(colour) / RGB_MAX * RGB16_MAX));
}

// Note, this is faster when not inlined when flto is enabled. Don't try to beat the compiler here.
u8 Colour::LerpU8(u8 a, u8 b, Colour::tColourLerpT t)
{
	int delta = (int)b - (int)a;
	int deltaT = (t * Colour::tColourLerpT(delta)).ToInt();
	return a + deltaT;
}

Colour::Colour()
	: r(RGB_MAX)
	, g(RGB_MAX)
	, b(RGB_MAX)
	, a(RGB_MAX)
{
}

Colour::Colour(u8 r, u8 g, u8 b, u8 a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
}

Colour::Colour(u8 r, u8 g, u8 b) 
	: r(r)
	, g(g)
	, b(b)
	, a(RGB_MAX)
{
}

Colour::~Colour()
{
}
