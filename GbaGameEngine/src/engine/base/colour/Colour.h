#pragma once
#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/math/Math.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "gbatek/ColourPalettes.h"

struct ColourRgb16Decompressed
{
	u8 r, g, b;
};

class Colour
{
public:
	using tColourLerpT = tFixedPoint24;

private:
	static u8 ScaleToMaxRgb16(u8 colour);
	static u8 LerpU8(u8 a, u8 b, Colour::tColourLerpT t);

public:
	u8 r, g, b, a;

	Colour();
	Colour(u8 r, u8 g, u8 b, u8 a);
	Colour(u8 r, u8 g, u8 b);
	~Colour();

	GBATEK::ColourRGB16 RGB16() const { return { ScaleToMaxRgb16(r), ScaleToMaxRgb16(g), ScaleToMaxRgb16(b) }; }

	const static Colour White;
	const static Colour Black;
	const static Colour Red;
	const static Colour Green;
	const static Colour Blue;

	static GBATEK::ColourRGB16 LerpRgb16(const ColourRgb16Decompressed& from, const ColourRgb16Decompressed& to, tColourLerpT t)
	{
		// Lerp
		u8 r = LerpU8(from.r, to.r, t);
		u8 g = LerpU8(from.g, to.g, t);
		u8 b = LerpU8(from.b, to.b, t);

		// Recompress
		return GBATEK::ColourRGB16{ r, g, b };
	}

	static inline ColourRgb16Decompressed DecompressRgb16(GBATEK::ColourRGB16 rgbColour)
	{
		ColourRgb16Decompressed colour;
		colour.r = rgbColour.r;
		colour.g = rgbColour.g;
		colour.b = rgbColour.b;

		return colour;
	}

	static inline GBATEK::ColourRGB16 LerpRgb16(GBATEK::ColourRGB16 from, GBATEK::ColourRGB16 to, tColourLerpT t)
	{
		if (from == to) return from;

		ColourRgb16Decompressed decompressedColourA = DecompressRgb16(from);
		ColourRgb16Decompressed decompressedColourB = DecompressRgb16(to);

		return LerpRgb16(decompressedColourA, decompressedColourB, t);
	}
};
