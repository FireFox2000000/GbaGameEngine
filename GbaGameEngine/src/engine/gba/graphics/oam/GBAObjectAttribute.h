#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/math/Math.h"
#include "engine/math/Vector2.h"
#include "engine/gba/graphics/GBADrawPriority.h"

// http://www.coranac.com/tonc/text/regobj.htm#tbl-obj-size

namespace GBA
{
	namespace Gfx
	{
		namespace Attributes
		{
			enum ObjectMode
			{
				ObjNormal,
				ObjAffine,
				ObjHidden,
				ObjAffineDoubleRendering
			};

			enum GfxMode
			{
				GfxNormal,
				GfxAlphaBlend,
				GfxWindowMask
			};

			enum ColourMode
			{
				FourBitsPerPixel,
				EightBitsPerPixel
			};

			enum Shape
			{
				Square,
				Wide,
				Tall,

				ShapeCount
			};

			enum SizeMode
			{
				Form0,
				Form1,
				Form2,
				Form3,

				SizeCount
			};
		}

		// Standard GBA hardware sprite. Fixed size, non-rotatable etc. Smaller VRam overhead. 
		// The GBA can draw a max of 128 of these at a time.
		// See https://www.coranac.com/tonc/text/regobj.htm for more details
		union ObjectAttribute
		{
		private:
			friend class OAMManager;

			struct Attr
			{
				u16 yCoord : 8
					, objMode : 2
					, gfxMode : 2
					, mosaic : 1
					, colourMode : 1
					, shape : 2
					;
				u16 xCoord : 9
					, : 3
					, horizontalFlip : 1
					, verticalFlip : 1
					, sizeMode : 2
					;
				u16 tileId : 10
					, priority : 2
					, paletteBank : 4
					;
				u16 : 16;
			};

			struct AffineAttr
			{
				u16 : 16;
				u16 : 9
					, affineIndex : 5	// Only used if ObjAffine is set to objMode
					, : 2
					;
				u16 : 16;
				u16 : 16;
			};

			Attr attributes;
			AffineAttr affineModeAttr;
			u16 data[4];		// This exists to zero out the attributes quicker than memset

		public:
			ObjectAttribute();

			void Reset() volatile;

			inline void SetPosition(const Vector2f& position) volatile	// Top-left of the sprite
			{
				SetXPosition(ROUND(position.x));
				SetYPosition(ROUND(position.y));
			}

			template<class IntType, u8 BITS>
			inline void SetPosition(const Vector2<FixedPoint<IntType, BITS> >& position) volatile	// Top-left of the sprite
			{
				SetXPosition(position.x.ToRoundedInt());
				SetYPosition(position.y.ToRoundedInt());
			}

			void SetXPosition(int value) volatile;
			void SetYPosition(int value) volatile;

			void SetObjectMode(Attributes::ObjectMode objectMode) volatile;
			void SetGfxMode(Attributes::GfxMode gfxMode) volatile;
			void SetColourMode(Attributes::ColourMode colourMode) volatile;
			void SetShape(Attributes::Shape shape) volatile;
			void SetSizeMode(Attributes::SizeMode sizeMode)	volatile;
			void SetPriority(DrawPriority layerNum) volatile;
			void SetTileIndex(u16 index) volatile;
			void SetPaletteIndex(u8 index) volatile;
			void SetAffineIndex(u8 index) volatile;

			void SetMosaic(bool enabled) volatile;

			void SetFlippedHorizontal() volatile;
			void SetFlippedVertical() volatile;

			void ClearFlippedHorizontal() volatile;
			void ClearFlippedVertical() volatile;

		} ALIGN(4);
	}
}
