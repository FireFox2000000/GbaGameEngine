#include "Colour.h"
#include "../Macros.h"

Colour::Colour()
	: r(0)
	, g(0)
	, b(0)
	, a(0)
{
}

Colour::Colour(u8 r, u8 g, u8 b) : Colour(r, g, b, 0)
{
}

Colour::Colour(u8 r, u8 g, u8 b, u8 a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
}

Colour::~Colour()
{
}

//////////////////////////////////////////////////////////

#define COL_16_MAX 31

Colour16::Colour16() : Colour()
{
}

Colour16::Colour16(u8 r, u8 g, u8 b) : Colour(MIN(r, COL_16_MAX), MIN(g, COL_16_MAX), MIN(b, COL_16_MAX))
{
}
