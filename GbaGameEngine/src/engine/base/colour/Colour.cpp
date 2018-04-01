#include "Colour.h"

Colour::Colour()
	: r(1)
	, g(1)
	, b(1)
	, a(1)
{
}

Colour::Colour(float r, float g, float b, float a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
}

Colour::Colour(float r, float g, float b) 
	: r(r)
	, g(g)
	, b(b)
	, a(1)
{
}

Colour::~Colour()
{
}
