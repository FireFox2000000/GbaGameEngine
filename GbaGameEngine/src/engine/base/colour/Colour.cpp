#include "Colour.h"

Colour::Colour()
	: r(1)
	, g(1)
	, b(1)
	, a(1)
{
}

Colour::Colour(u8 r, u8 g, u8 b) : Colour(r, g, b, 1)
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