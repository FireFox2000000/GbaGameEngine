#pragma once

#include "engine/math/Vector2.h"
#include "engine/base/Macros.h"
#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Bitmask.h"

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

			// Available backgrounds
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

	// https://www.coranac.com/tonc/text/video.htm
	class DisplayControl
	{
		static Vector2<int> m_screenResolution;

	public:
		static void SetDisplayOptions(int params);
		static Vector2<int> GetScreenResolution();

		static DisplayOptions::VideoMode GetVideoMode();
		static DisplayOptions::SpriteMappingMode GetSpriteMappingMode();
		static bool TestObjectRendering(DisplayOptions::ObjectRendering val);

		static void SetBackgroundActive(int backgroundIndex, bool active);

		static Bitmask<u8> GetBackgroundsForCurrentVideoMode();
	};
}
