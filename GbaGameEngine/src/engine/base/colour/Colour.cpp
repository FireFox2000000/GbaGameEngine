#include "Colour.h"

const Colour Colour::White = Colour(Colour::RGB_MAX, Colour::RGB_MAX, Colour::RGB_MAX);
const Colour Colour::Black = Colour(0, 0, 0);
const Colour Colour::Red = Colour(Colour::RGB_MAX, 0, 0);
const Colour Colour::Green = Colour(0, Colour::RGB_MAX, 0);
const Colour Colour::Blue = Colour(0, 0, Colour::RGB_MAX);

Colour::Colour()
	: r(Colour::RGB_MAX)
	, g(Colour::RGB_MAX)
	, b(Colour::RGB_MAX)
	, a(Colour::RGB_MAX)
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
	, a(Colour::RGB_MAX)
{
}

Colour::~Colour()
{
}
