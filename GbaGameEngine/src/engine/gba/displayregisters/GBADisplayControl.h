#pragma once
#include "../../math/Vector2.h"
#include "../../base/Macros.h"

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

		//static int SpriteMapping1D = BIT(6);
	}

	class DisplayControl
	{
	public:		
		static void SetDisplayOptions(int params);

		static DisplayOptions::VideoMode GetVideoMode();
		static Vector2 GetScreenResolution();
	};
}
