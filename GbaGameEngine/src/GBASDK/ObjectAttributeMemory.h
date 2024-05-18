#pragma once
#include "internal/Internal.h"

namespace GBA
{
	enum class ObjectMode : unsigned short
	{
		Normal,
		Affine,
		Hidden,
		AffineDoubleRendering
	};

	enum class GfxMode : unsigned short
	{
		Normal,
		AlphaBlend,
		WindowMask
	};

	enum class MosaicState : unsigned short
	{
		Off,
		On
	};

	enum class ColourMode : unsigned short
	{
		FourBitsPerPixel,
		EightBitsPerPixel
	};

	enum class ObjectShape : unsigned short
	{
		Square,
		Wide,
		Tall
	};

	enum class ObjectSize : unsigned short
	{
		// shape\size	00	01	10	11
		// 00	8x8		16x16	32x32	64x64
		// 01	16x8	32x8	32x16	64x32
		// 10	8x16	8x32	16x32	32x64
		Smallest,
		Small,
		Large,
		Largest
	};

	enum class ObjectFlippedState : unsigned short
	{
		Normal,
		Mirrored
	};

	struct ObjectAttribute
	{
		/*** Attr 0 ***/

		// 0 - 255
		unsigned short screenPosY : 8;
		ObjectMode objectMode : 2;
		GfxMode gfxMode : 2;
		MosaicState mosaic : 1;
		ColourMode colourMode : 1;
		ObjectShape shape : 2;

		/*** Attr 1 ***/

		union
		{
			// Disable "warning GAB0A7ABD: ISO C++ prohibits anonymous structs [-Wpedantic]"
GBA_DIAGNOSTIC_PUSH_IGNORE_WARNING_ANON_STRUCTS
			struct
			{
				// 0 - 511
				unsigned short screenPosX : 9;	
				unsigned short : 3;
				// Only available if objectMode != ObjectMode::Affine
				ObjectFlippedState flipHorizontal : 1;
				// Only available if objectMode != ObjectMode::Affine
				ObjectFlippedState flipVertical : 1;
				ObjectSize size : 2;
			};
			struct
			{
				unsigned short : 9;
				// 0 - 31
				// Affine index is only used if objectMode == ObjectMode::Affine
				unsigned short affineIndex : 5;
				unsigned short : 2;
			};
GBA_DIAGNOSTIC_POP
		};

		/*** Attr 2 ***/
		
		// 0 - 1023
		// Index of vram::objectTiles (BaseTile) array that the tile data was loaded into. 
		// Note that in bitmap modes 3-5 this must be 512 or higher.
		unsigned short baseTileIndex : 10;

		// 0 - 3
		// 0 = Highest
		unsigned short priority : 2;

		// Has no effect if the colorMode == EightBitsPerPixel
		unsigned short palleteBankIndex : 4;

		/*** Unused, ObjectAttributeAffine data ***/
		unsigned short : 16;

	} GBA_ALIGN(4);	// See https://www.coranac.com/tonc/text/bitmaps.htm#ssec-data-align

	class ObjectAttributeAffine
	{
	private:
		// fills map to ObjectAttribute properties.
		unsigned short fill0[3];
	public:
		short paFixedPoint8;
	private:
		// fills map to ObjectAttribute properties.
		unsigned short fill1[3];
	public:
		short pbFixedPoint8;
	private:
		// fills map to ObjectAttribute properties.
		unsigned short fill2[3];
	public:
		short pcFixedPoint8;
	private:
		// fills map to ObjectAttribute properties.
		unsigned short fill3[3];
	public:
		short pdFixedPoint8;
	} GBA_ALIGN(4);	// See https://www.coranac.com/tonc/text/bitmaps.htm#ssec-data-align

	static_assert(sizeof(ObjectAttribute) == 8, "ObjectAttribute struct malformed");
	static_assert(sizeof(ObjectAttributeAffine) == 32, "ObjectAttributeAffine struct malformed");

	union UObjectAttributeMemory
	{
		ObjectAttribute attributes[128];
		ObjectAttributeAffine affineAttributes[32];
	};
	
	UObjectAttributeMemory* const objectAttributeMemory = reinterpret_cast<UObjectAttributeMemory* const>(0x07000000);
}

#undef GBA_ALIGN
