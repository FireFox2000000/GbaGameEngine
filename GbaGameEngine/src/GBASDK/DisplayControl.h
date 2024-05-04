#pragma once
namespace GBA
{
	enum class VideoMode : unsigned char
	{
		Mode0,	// Tile mode
		Mode1,	// Tile mode
		Mode2,	// Tile mode

		Mode3,	// Bitmap mode
		Mode4,	// Bitmap mode
		Mode5	// Bitmap mode

		// Available backgrounds
		/*	mode	|	BG0	|	BG1	|	BG2	|	BG3
		*	0		|	reg	|	reg	|	reg	|	reg
		*	1		|	reg	|	reg	|	aff	|	-
		*	2		|	-	|	-	|	aff	|	aff
		*/
	};

	enum class CGBMode : unsigned char
	{
		GameBoyAdvance,
		GameBoyColour
	};

	enum class ObjectMappingMode : unsigned char
	{
		TwoDimensional,
		OneDimensional
	};

	struct DisplayControl
	{
		VideoMode videoMode : 3;

		// Has a game boy colour cart been inserted
		const CGBMode cgbMode : 1;

		// VideoMode 4 and 5 only. These can use page flipping for smoother animation. 
		// This bit selects the displayed page (and allowing the other one to be drawn on without artifacts).
		bool pageSelectEnabled : 1;

		// Allows access to OAM in an HBlank. OAM is normally locked in VDraw. Will reduce the amount of sprite pixels rendered per line.
		bool oamHBlankEnabled : 1;

		ObjectMappingMode objectMappingMode : 1;

		// Allow FAST access to VRAM, Palette and OAM. Appears to be enabled by default. 
		bool forceScreenBlank : 1;

		bool enableBackground0 : 1;
		bool enableBackground1 : 1;
		bool enableBackground2 : 1;
		bool enableBackground3 : 1;

		bool enableSprites : 1;

		bool enableWindow0 : 1;
		bool enableWindow1 : 1;

		bool enableWindowObject : 1;

		inline void SetBackgroundEnabled(int index, bool isEnabled)
		{
			switch (index)
			{
			case 0:
			{
				enableBackground0 = isEnabled;
				break;
			}
			case 1:
			{
				enableBackground1 = isEnabled;
				break;
			}
			case 2:
			{
				enableBackground2 = isEnabled;
				break;
			}
			case 3:
			{
				enableBackground3 = isEnabled;
				break;
			}
			}
		}
	};

	// Read/write
	// Note that the default state for this register appears to have the forceScreenBlank bit set
	// Recommended to set to false upon initialisation
	DisplayControl* const ioRegisterDisplayControl = reinterpret_cast<DisplayControl*>(0x4000000);
}