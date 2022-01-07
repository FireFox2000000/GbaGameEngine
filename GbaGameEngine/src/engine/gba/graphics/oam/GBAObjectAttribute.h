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
		class ObjectAttribute
		{
			friend class OAMManager;

			u16 m_attributeZero;
			u16 m_attributeOne;
			u16 m_attributeTwo;
			u16 padding;

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
			void SetTileIndex(u32 index) volatile;
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
