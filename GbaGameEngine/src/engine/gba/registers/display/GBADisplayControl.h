#pragma once

#include "engine/math/Vector2.h"
#include "engine/base/Macros.h"
#include "engine/base/Typedefs.h"

// http://www.coranac.com/tonc/text/video.htm

namespace GBA
{
	namespace DisplayOptions
	{
		
		enum VideoMode
		{
			Mode0 = 0x0,	// Tile mode
			Mode1 = 0x1,	// Tile mode
			Mode2 = 0x2,	// Tile mode

			Mode3 = 0x3,	// Bitmap mode
			Mode4 = 0x4,	// Bitmap mode
			Mode5 = 0x5		// Bitmap mode

			/*	mode	|	BG0	|	BG1	|	BG2	|	BG3
			*	0		|	reg	|	reg	|	reg	|	reg
			*	1		|	reg	|	reg	|	aff	|	-
			*	2		|	-	|	-	|	aff	|	aff
			*/
		};

		enum ObjectRendering
		{
			Background0 = BIT(8),
			Background1 = BIT(9),
			Background2 = BIT(0xA),
			Background3 = BIT(0xB),
			Sprites = BIT(0xC)
		};

		enum SpriteMappingMode
		{
			MappingMode2D = 0,
			MappingMode1D = BIT(6)
		};
	}

	class DisplayControl
	{
		static vu32& s_REG_DISPCNT;
		static Vector2<int> m_screenResolution;

	public:		
		static void SetDisplayOptions(int params);
		static inline Vector2<int> GetScreenResolution() { return m_screenResolution; }

		inline static DisplayOptions::VideoMode GetVideoMode() { return DisplayOptions::VideoMode(s_REG_DISPCNT & BITS_INDEXED_U32(3, 0)); }
		inline static DisplayOptions::SpriteMappingMode GetSpriteMappingMode() { return DisplayOptions::SpriteMappingMode(s_REG_DISPCNT & BIT(6)); }
	};
}
