#define EWRAM_DATA __attribute__((section(".ewram")))

namespace __binary_spritesheet_Player 
{
	// File Header
	extern EWRAM_DATA const unsigned long spriteCount = 1;
	extern EWRAM_DATA const unsigned char paletteLength = 5;
	extern EWRAM_DATA const unsigned long dataLength = 32;

	// Bit0 - 3   Data size in bit units(normally 4 or 8). May be reserved/unused for other compression types 
	// Bit4-7   Compressed type 
	// Bit8-31  Unused, generated in-game as 24bit size of decompressed data in bytes, probably 
	extern EWRAM_DATA const unsigned long compressionTypeSize = 148; 

	extern EWRAM_DATA const unsigned short palette[] = 
	{
		0x0000, 0x7FFF, 0x0842, 0x4E73, 0x739C, 
		
	};

	extern EWRAM_DATA const unsigned char widthMap[] = 
	{
		16, 
	};

	extern EWRAM_DATA const unsigned char heightMap[] = 
	{
		16, 
	};

	extern EWRAM_DATA const unsigned long offsets[] = 
	{
		0, 
	};

	extern EWRAM_DATA const unsigned long data[] = 
	{
		0x22211111, 0x33321111, 0x33332111, 0x33332111, 0x44322211, 
		0x22242211, 0x44442421, 0x42444421, 0x11111222, 0x11112333, 
		0x11123333, 0x11123333, 0x11222344, 0x11224222, 0x12424444, 
		0x12444424, 0x42442211, 0x34422211, 0x22224421, 0x22224421, 
		0x23322211, 0x32232111, 0x23332111, 0x12221111, 0x11224424, 
		0x11222443, 0x12442222, 0x12442222, 0x11222332, 0x11123223, 
		0x11123332, 0x11112221, 

	};

}
