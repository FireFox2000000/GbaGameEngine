#pragma once
#include "ColourPalettes.h"
#include "internal/Internal.h"

namespace GBA
{
	// Cannot write to vram in byte-size, reads are fine. 
	// Writes must be performed in 2 or 4 byte sizes.

	static constexpr int TILES_PER_BLOCK = 512;
	static constexpr int PIXELS_PER_TILE = 8 * 8;
	static constexpr int BYTES_PER_TILE_4BBP = PIXELS_PER_TILE / 2;

	union UPixelData
	{
		// i.e. 4 bits-per-pixel for a total of 16 colours from the palette to pick from
		struct ColourMode16
		{
			unsigned int pixel0PaletteIndex : 4;
			unsigned int pixel1PaletteIndex : 4;
			unsigned int pixel2PaletteIndex : 4;
			unsigned int pixel3PaletteIndex : 4;
			unsigned int pixel4PaletteIndex : 4;
			unsigned int pixel5PaletteIndex : 4;
			unsigned int pixel6PaletteIndex : 4;
			unsigned int pixel7PaletteIndex : 4;
		} colourMode16;
		
		// i.e. 8 bits per-pixel for a total of 255 colours from the palette to pick from
		struct ColourMode255
		{
			unsigned int pixel0PaletteIndex : 8;
			unsigned int pixel1PaletteIndex : 8;
			unsigned int pixel2PaletteIndex : 8;
			unsigned int pixel3PaletteIndex : 8;
		} colourMode255;
	};

	struct BaseTile
	{
		// 8 x 8 pixels in 16 colour mode, or 8 x 4 pixels in 255 colour mode
		UPixelData pixelData[8];
	};

	enum class BackgroundTileFlippedState : unsigned short
	{
		Normal,
		Mirrored
	};

	struct BackgroundTilemapEntry
	{
		unsigned short tileIndex : 10;
		BackgroundTileFlippedState flippedHorizontal : 1;
		BackgroundTileFlippedState flippedVertical : 1;
		// Palette bank to use when in 16-color mode. Has no effect for 256-color bgs.
		unsigned short paletteBankIndex : 4;
	};

	static_assert(sizeof(BackgroundTilemapEntry) == 2, "BackgroundTilemapEntry struct malformed");
	
	// Modes 0-2
	struct VramTileMode
	{
		union UBackgroundMapsAndTiles
		{
			// Where tile data is stored
			// Total of 4 character base blocks, however data usage can overlap.
			// Backgrounds will load tile data at the start of one of the 4 blocks. 
			BaseTile characterBaseBlocks[4][TILES_PER_BLOCK];

			// Where map data is stored
			// Map data can be loaded between an index of 0-32 in any slots not taken up 
			// by tiles already loaded in backgroundTiles.
			// Every background aside from the smallest will need to be written across 
			// multiple screen base blocks
			BackgroundTilemapEntry screenBaseBlocks[32][1024];

			static_assert(sizeof(characterBaseBlocks) == GBA_KILOBYTES_TO_BYTES(64),
				"characterBaseBlocks size must be 64Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
			static_assert(sizeof(screenBaseBlocks) == GBA_KILOBYTES_TO_BYTES(64),
				"screenBaseBlocks size must be 64Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
		};

		// Union, map entries and tile data is stored in the same location.
		UBackgroundMapsAndTiles backgroundMapsAndTiles;

		// Total of 2 character base blocks for a total of 1024 tiles
		BaseTile objectTiles[TILES_PER_BLOCK * 2];

		static unsigned int constexpr ScreenBaseBlockToCharacterBaseBlock(unsigned int sbb)
		{
			return sbb / 8;
		}

		static unsigned int constexpr CharacterBaseBlockToScreenBaseBlock(unsigned int cbb)
		{
			return cbb * 8;
		}

		static_assert(sizeof(backgroundMapsAndTiles) == GBA_KILOBYTES_TO_BYTES(64),
			"BackgroundMapsAndTiles size must be 64Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
		static_assert(sizeof(objectTiles) == GBA_KILOBYTES_TO_BYTES(32), 
			"ObjectTiles size must be 32Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
	};

	// 16 bits per-pixels at 240 * 160 resolution
	// No page-flipping/double buffering available
	struct VramBitmapMode3
	{
		ColourRGB16 frameBuffer[240 * 160];

	private:
		char unusedFrameBuffer[GBA_KILOBYTES_TO_BYTES(5)];

	public:
		BaseTile objectTiles[TILES_PER_BLOCK];

		static_assert(sizeof(frameBuffer) + sizeof(unusedFrameBuffer) == GBA_KILOBYTES_TO_BYTES(80),
			"Frame buffer size must be 80Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
		static_assert(sizeof(frameBuffer) + sizeof(unusedFrameBuffer) == GBA_KILOBYTES_TO_BYTES(80),
			"Frame buffer size must be 80Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
		static_assert(sizeof(objectTiles) == GBA_KILOBYTES_TO_BYTES(16),
			"ObjectTiles size must be 16Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
	};

	// Colours use 8 bits per-pixel, and is an index to the colour stored
	// at GBA::colourPaletteBackgrounds[0]
	struct VramBitmapMode4
	{
		// Read-only, must write in blocks of 16 or 32 bits
		char frameBuffer0[240 * 160];
	private:
		char unusedFrameBuffer0[GBA_KILOBYTES_TO_BYTES(2.5f)];

	public:
		// Read-only, must write in blocks of 16 or 32 bits
		char frameBuffer1[240 * 160];
	private:
		char unusedFrameBuffer1[GBA_KILOBYTES_TO_BYTES(2.5f)];

	public:
		BaseTile objectTiles[TILES_PER_BLOCK];

		static_assert(sizeof(frameBuffer0) + sizeof(unusedFrameBuffer0) == GBA_KILOBYTES_TO_BYTES(40),
			"Frame buffer 0 size must be 40Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
		static_assert(sizeof(frameBuffer1) + sizeof(unusedFrameBuffer1) == GBA_KILOBYTES_TO_BYTES(40),
			"Frame buffer 1 size must be 40Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
		static_assert(sizeof(objectTiles) == GBA_KILOBYTES_TO_BYTES(16),
			"ObjectTiles size must be 16Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
	};

	// 16 bits per-pixel similar to mode 3, however does not cover the entire screen
	// Allows for page flipping/double buffering
	// https://www.coranac.com/tonc/img/demo/bm_modes_5.png
	struct VramBitmapMode5
	{
		ColourRGB16 frameBuffer0[160 * 128];
		ColourRGB16 frameBuffer1[160 * 128];

		// Total of 1 character base block for a total of 512 tiles.
		// Note that GBA::ObjectAttribute::tileId must be 512 + this index
		BaseTile objectTiles[TILES_PER_BLOCK];

		static_assert(sizeof(frameBuffer0) == GBA_KILOBYTES_TO_BYTES(40),
			"Frame buffer 0 size must be 40Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
		static_assert(sizeof(frameBuffer1) == GBA_KILOBYTES_TO_BYTES(40),
			"Frame buffer 1 size must be 40Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
		static_assert(sizeof(objectTiles) == GBA_KILOBYTES_TO_BYTES(16),
			"ObjectTiles size must be 16Kb - https://www.akkit.org/info/gbatek.htm#lcdvramoverview");
	};

	union UVram
	{
		// Used in GBA::VideoMode::Mode0
		VramTileMode videoMode0;

		// Used in GBA::VideoMode::Mode1
		VramTileMode videoMode1;

		// Used in GBA::VideoMode::Mode2
		VramTileMode videoMode2;

		// Used in GBA::VideoMode::Mode3
		VramBitmapMode3 videoMode3;

		// Used in GBA::VideoMode::Mode4
		VramBitmapMode4 videoMode4;

		// Used in GBA::VideoMode::Mode5
		VramBitmapMode5 videoMode5;
	};

	UVram* const vram = reinterpret_cast<UVram*>(0x06000000);
}
