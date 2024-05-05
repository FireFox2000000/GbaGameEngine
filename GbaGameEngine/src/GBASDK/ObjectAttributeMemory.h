#pragma once
#include "array.h"
#include "engine/base/core/stl/FixedPoint.h"

namespace GBA
{
	enum class ObjectMode : unsigned
	{
		Normal,
		Affine,
		Hidden,
		AffineDoubleRendering
	};

	enum class GfxMode : unsigned
	{
		Normal,
		AlphaBlend,
		WindowMask
	};

	enum class ColourMode : unsigned
	{
		FourBitsPerPixel,
		EightBitsPerPixel
	};

	enum class ObjectShape : unsigned
	{
		Square,
		Wide,
		Tall
	};

	enum class ObjectSize : unsigned
	{
		// shape\size	00	01	10	11
		// 00	8x8		16x16	32x32	64x64
		// 01	16x8	32x8	32x16	64x32
		// 10	8x16	8x32	16x32	32x64
		Form0,
		Form1,
		Form2,
		Form3,

		SizeCount
	};

	struct ObjectAttribute
	{
		/*** Attr 0 ***/

		signed char yCoordinate : 8;
		ObjectMode objectMode : 2;
		GfxMode gfxMode : 2;
		bool mosaicEnabled : 1;
		ColourMode colourMode : 1;
		ObjectShape shape : 2;

		/*** Attr 1 ***/

		signed char xCoordinate : 8;
		union
		{
			// 0 - 31
			// Affine index is only used if objectMode == ObjectMode::Affine
			unsigned char affineIndex : 5;

			// Otherwise if cleared, the next 3 bits are unused
			// with the remaining 2 controlling flipping parameters
			struct {
				char : 2;
				bool flipHorizontal;
				bool flipVertical;
			};
		};
		ObjectSize size;

		/*** Attr 2 ***/
		
		// 0 - 1023
		// Base tile-index of sprite. Note that in bitmap modes this must be 512 or higher.
		unsigned short tileId : 10;

		// 0 - 3
		// 0 = Highest
		unsigned char priority : 2;

		// Has no effect if the colorMode == EightBitsPerPixel
		unsigned char palleteBankIndex : 4;

		/*** Unused, affine data ***/
		unsigned short : 16;

	} __attribute__((aligned(4)));

	class ObjectAttributeAffine
	{
		using FixedPoint8 = FixedPoint<s16, 8>;

	private:
		// fills map to ObjectAttribute properties. DO NOT TOUCH THESE. Only need to fill out pa-pd.
		u16 fill0[3];
		FixedPoint8 pa;
		u16 fill1[3];
		FixedPoint8 pb;
		u16 fill2[3];
		FixedPoint8 pc;
		u16 fill3[3];
		FixedPoint8 pd;

	public:
		ObjectAttributeAffine() : pa(0), pb(0), pc(0), pd(0) {}

		// Don't trash fills
		//void SetTransformation(const Matrix2x2& that) volatile
		//{
		//	pa.SetStorage(that.a.GetStorage());
		//	pb.SetStorage(that.b.GetStorage());
		//	pc.SetStorage(that.c.GetStorage());
		//	pd.SetStorage(that.d.GetStorage());
		//}
	} __attribute__((aligned(4));

	array<ObjectAttribute, 128>* const objectAttributes = reinterpret_cast<array<ObjectAttribute, 128>*>(0x07000000);

}
