#pragma once
namespace GBA
{
	struct ColourRGB16
	{
		// 0 - 31
		unsigned short r : 5;

		// 0 - 31
		unsigned short g : 5;

		// 0 - 31
		unsigned short b : 5;

		// This exists for fast colour comparisons and VRAM mem copies
		inline operator unsigned short() const { return *reinterpret_cast<const unsigned short*>(this); }
	};

	static_assert(sizeof(ColourRGB16) == sizeof(unsigned short), "ColourRGB16 struct malformed");
}
