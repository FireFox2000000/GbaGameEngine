#ifndef PRAGMA_ONCE_GBADISPLAY_CONTROL_H
#define PRAGMA_ONCE_GBADISPLAY_CONTROL_H

#include "../../../math/Vector2.h"
#include "../../../base/Macros.h"
#include "../../../base/Typedefs.h"

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

	public:		
		static void SetDisplayOptions(int params);
		static Vector2 GetScreenResolution();

		inline static DisplayOptions::VideoMode GetVideoMode() { return DisplayOptions::VideoMode(s_REG_DISPCNT & BITS_INDEXED_U32(3, 0)); }
		inline static DisplayOptions::SpriteMappingMode GetSpriteMappingMode() { return DisplayOptions::SpriteMappingMode(s_REG_DISPCNT & BIT(6)); }
	};
}

#endif
