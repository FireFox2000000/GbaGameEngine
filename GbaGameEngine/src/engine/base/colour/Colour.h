#pragma once
#include "../Typedefs.h"

struct Colour
{
	u8 r, g, b, a;

public:
	Colour();
	Colour(u8 r, u8 g, u8 b);
	Colour(u8 r, u8 g, u8 b, u8 a);
	~Colour();
};

//////////////////////////////////////////////////////////

struct Colour16 : Colour
{
public:
	Colour16();
	Colour16(u8 r, u8 g, u8 b);		// Values of 0-31 per colour

	inline u16 RGB16() const { return ((r)+((g) << 5) + ((b) << 10)); }
};

