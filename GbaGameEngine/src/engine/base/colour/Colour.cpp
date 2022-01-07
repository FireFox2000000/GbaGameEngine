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

// Safe version
Rgb16 Colour::RGB16(u8 r, u8 g, u8 b)
{
	return Rgb16(MIN(r, RGB16_MAX), (MIN(g, RGB16_MAX)), (MIN(b, RGB16_MAX)));
}		
/*
Rgb16 Colour::LerpRgb16(Rgb16 from, Rgb16 to, Colour::tColourLerpT t)
{
	if (from == to) return from;

	constexpr u8 mask5Bit = BITS_U32(5);

	// Decompress
	u8 ar = mask5Bit & from;
	u8 br = mask5Bit & to;

	u8 ag = mask5Bit & (from >> 5);
	u8 bg = mask5Bit & (to >> 5);

	u8 ab = mask5Bit & (from >> 10);
	u8 bb = mask5Bit & (to >> 10);

	// Lerp
	u8 r = LerpRgb16Prop(ar, br, t);
	u8 g = LerpRgb16Prop(ag, bg, t);
	u8 b = LerpRgb16Prop(ab, bb, t);

	// Recompress
	return MAKE_RGB16(r, g, b);
}*/

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

ColourRgb16Decompressed Colour::DecompressRgb16(Rgb16 rgbColour)
{
	ColourRgb16Decompressed colour;
	colour.r = rgbColour.r;
	colour.g = rgbColour.g;
	colour.b = rgbColour.b;

	return colour;
}

Rgb16::Rgb16(u16 val)
{
	*(u16*)this = val;
}

Rgb16::Rgb16(u8 _r, u8 _g, u8 _b) : r(_r), g(_g), b(_b)
{
	static_assert(sizeof(Rgb16) == (sizeof(u16)), "Rgb16 not the correct size");
}

Rgb16::Rgb16(const Rgb16& that)
{
	*this = that;
}

bool Rgb16::operator==(const Rgb16& that)
{
	return *(u16*)this == (u16)that;
}

bool Rgb16::operator==(const volatile Rgb16& that) volatile
{
	return *(u16*)this == *(u16*)&that;
}

void Rgb16::operator=(const Rgb16& that)
{
	*(u16*)this = that;
}

void Rgb16::operator=(const Rgb16& that) volatile
{
	*(u16*)this = that;
}

Rgb16::operator u16() const
{
	return *(u16*)this;
}
