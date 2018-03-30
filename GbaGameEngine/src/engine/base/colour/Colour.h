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

	static inline u16 RGB15(u8 r, u8 g, u8 b) { return ((r)+((g) << 5) + ((b) << 10)); }
	inline u16 RGB15() const { return Colour::RGB15(r, g, b); }
};

