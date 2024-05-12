#pragma once

#define GBA_ALIGN(n) __attribute__((aligned(n)))

namespace GBA
{
	enum class ObjectMode : unsigned char
	{
		Normal,
		Affine,
		Hidden,
		AffineDoubleRendering
	};

	enum class GfxMode : unsigned char
	{
		Normal,
		AlphaBlend,
		WindowMask
	};

	enum class ColourMode : unsigned char
	{
		FourBitsPerPixel,
		EightBitsPerPixel
	};

	enum class ObjectShape : unsigned char
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
		Form0,
		Form1,
		Form2,
		Form3
	};

	struct ObjectAttribute
	{
		/*** Attr 0 ***/

		// 0 - 255
		unsigned char screenPosY : 8;
		ObjectMode objectMode : 2;
		GfxMode gfxMode : 2;
		bool mosaicEnabled : 1;
		ColourMode colourMode : 1;
		ObjectShape shape : 2;

		/*** Attr 1 ***/

		union
		{
			struct
			{
				// 0 - 511
				unsigned short screenPosX : 9;	
				unsigned short : 3;
				// Only available if objectMode != ObjectMode::Affine
				unsigned short flipHorizontal : 1;
				// Only available if objectMode != ObjectMode::Affine
				unsigned short flipVertical : 1;
				ObjectSize size : 2;
			};
			struct
			{
				bool : 8;
				bool : 1;
				// 0 - 31
				// Affine index is only used if objectMode == ObjectMode::Affine
				unsigned char affineIndex : 5;
				bool : 2;
			};
		};

		/*** Attr 2 ***/
		
		// 0 - 1023
		// Base tile-index of sprite. Note that in bitmap modes this must be 512 or higher.
		unsigned short tileId : 10;

		// 0 - 3
		// 0 = Highest
		unsigned short priority : 2;

		// Has no effect if the colorMode == EightBitsPerPixel
		unsigned short palleteBankIndex : 4;

		/*** Unused, affine data ***/
		unsigned short : 16;

	} GBA_ALIGN(4);	// See https://www.coranac.com/tonc/text/bitmaps.htm#ssec-data-align

	class ObjectAttributeAffine
	{
	private:
		// fills map to ObjectAttribute properties. DO NOT TOUCH THESE. Only need to fill out pa-pd.
		unsigned short fill0[3];
		short paFixedPoint8;
		unsigned short fill1[3];
		short pbFixedPoint8;
		unsigned short fill2[3];
		short pcFixedPoint8;
		unsigned short fill3[3];
		short pdFixedPoint8;

	public:
		ObjectAttributeAffine() : paFixedPoint8(0), pbFixedPoint8(0), pcFixedPoint8(0), pdFixedPoint8(0) {}
	} GBA_ALIGN(4);	// See https://www.coranac.com/tonc/text/bitmaps.htm#ssec-data-align

	static_assert(sizeof(ObjectAttribute) == 8, "ObjectAttribute struct malformed");
	static_assert(sizeof(ObjectAttributeAffine) == 32, "ObjectAttributeAffine struct malformed");

	union UObjectAttributes
	{
		ObjectAttribute attributes[128];
		ObjectAttributeAffine affineAttributes[32];
	};
	
	UObjectAttributes* const objectAttributeMemory = reinterpret_cast<UObjectAttributes* const>(0x07000000);
}

#undef GBA_ALIGN
