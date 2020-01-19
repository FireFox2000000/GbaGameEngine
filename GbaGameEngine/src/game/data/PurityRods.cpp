#define EWRAM_DATA __attribute__((section(".ewram")))

namespace __binary_spritesheet_PurityRods 
{
	// File Header
	extern EWRAM_DATA const unsigned long spriteCount = 3;
	extern EWRAM_DATA const unsigned char paletteLength = 6;
	extern EWRAM_DATA const unsigned long dataLength = 96;

	// Bit0 - 3   Data size in bit units(normally 4 or 8). May be reserved/unused for other compression types 
	// Bit4-7   Compressed type 
	// Bit8-31  Unused, generated in-game as 24bit size of decompressed data in bytes, probably 
	extern EWRAM_DATA const unsigned long compressionTypeSize = 148; 

	extern EWRAM_DATA const unsigned short palette[] = 
	{
		0x0000, 0x0000, 0x008C, 0x23BF, 0x7FFF, 
		0x3F3F, 
	};

	extern EWRAM_DATA const unsigned char widthMap[] = 
	{
		16, 16, 16, 
	};

	extern EWRAM_DATA const unsigned char heightMap[] = 
	{
		16, 16, 16, 
	};

	extern EWRAM_DATA const unsigned long offsets[] = 
	{
		0, 32, 64, 
	};

	extern EWRAM_DATA const unsigned long data[] = 
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
		0x00000000, 0x10000000, 0x21000000, 0x01100000, 0x01210000, 
		0x01421000, 0x01422100, 0x14112210, 0x01411221, 0x14100122, 
		0x01410012, 0x22100000, 0x12210000, 0x01221000, 0x00122100, 
		0x00012210, 0x00001221, 0x00000121, 0x00000010, 0x14100001, 
		0x01410000, 0x14100000, 0x01410000, 0x14100000, 0x01410000, 
		0x14100000, 0x01000000, 

		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
		0x00000000, 0x10000000, 0x21000000, 0x01100000, 0x01210000, 
		0x01521000, 0x01522100, 0x15112210, 0x01511221, 0x15100122, 
		0x01510012, 0x22100000, 0x12210000, 0x01221000, 0x00122100, 
		0x00012210, 0x00001221, 0x00000121, 0x00000010, 0x15100001, 
		0x01510000, 0x15100000, 0x01510000, 0x15100000, 0x01510000, 
		0x15100000, 0x01000000, 

		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
		0x00000000, 0x10000000, 0x31000000, 0x01100000, 0x01310000, 
		0x01431000, 0x01433100, 0x14113310, 0x01411331, 0x14100133, 
		0x01410013, 0x33100000, 0x13310000, 0x01331000, 0x00133100, 
		0x00013310, 0x00001331, 0x00000131, 0x00000010, 0x14100001, 
		0x01410000, 0x14100000, 0x01410000, 0x14100000, 0x01410000, 
		0x14100000, 0x01000000, 

	};

}
