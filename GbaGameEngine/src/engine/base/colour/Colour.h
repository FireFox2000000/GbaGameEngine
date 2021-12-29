#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/math/Math.h"
#include "engine/base/core/stl/FixedPoint.h"

typedef u16 Rgb16;

// No guards, RGB16 method
#define MAKE_RGB16(r, g, b) ((r) + ((g) << 5) + ((b) << 10))

class Colour
{
public:
	using tColourLerpT = tFixedPoint24;

private:
	static u8 ScaleToMaxRgb16(u8 colour);
	static u8 LerpU8(u8 a, u8 b, Colour::tColourLerpT t);

public:
	// Values must be between 0 and 31
	static Rgb16 RGB16(u8 r, u8 g, u8 b);

	static inline Rgb16 LerpRgb16(Rgb16 from, Rgb16 to, tColourLerpT t)
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
		u8 r = LerpU8(ar, br, t);
		u8 g = LerpU8(ag, bg, t);
		u8 b = LerpU8(ab, bb, t);

		// Recompress
		return MAKE_RGB16(r, g, b);
	}

	u8 r, g, b, a;

	Colour();
	Colour(u8 r, u8 g, u8 b, u8 a);
	Colour(u8 r, u8 g, u8 b);
	~Colour();

	Rgb16 RGB16() const { return Colour::RGB16(ScaleToMaxRgb16(r), ScaleToMaxRgb16(g), ScaleToMaxRgb16(b)); }

	const static Colour White;
	const static Colour Black;
	const static Colour Red;
	const static Colour Green;
	const static Colour Blue;
};
