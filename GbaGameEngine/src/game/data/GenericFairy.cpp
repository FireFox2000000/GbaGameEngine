#define EWRAM_DATA __attribute__((section(".ewram")))

namespace __binary_spritesheet_GenericFairy 
{
	// File Header
	extern EWRAM_DATA const unsigned long spriteCount = 1;
	extern EWRAM_DATA const unsigned char paletteLength = 6;
	extern EWRAM_DATA const unsigned long dataLength = 32;

	// Bit0 - 3   Data size in bit units(normally 4 or 8). May be reserved/unused for other compression types 
	// Bit4-7   Compressed type 
	// Bit8-31  Unused, generated in-game as 24bit size of decompressed data in bytes, probably 
	extern EWRAM_DATA const unsigned long compressionTypeSize = 148; 

	extern EWRAM_DATA const unsigned short palette[] = 
	{
		0x0000, 0x0000, 0x7EF6, 0x5BBF, 0x7FFF, 
		0x7EDB, 
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
		0x11100000, 0x22210000, 0x11221000, 0x33121000, 0x31310000, 
		0x31310010, 0x33310141, 0x11121441, 0x00000111, 0x00001222, 
		0x00012211, 0x00012133, 0x00001313, 0x01001313, 0x14101333, 
		0x14412111, 0x51221441, 0x13121441, 0x13114441, 0x11514411, 
		0x55551100, 0x55555100, 0x11111000, 0x00000000, 0x14412215, 
		0x14412131, 0x14441131, 0x01441511, 0x00115555, 0x00155555, 
		0x00011111, 0x00000000, 

	};

}
