#pragma once
#include "../../base/Typedefs.h"
#include "../../base/Macros.h"
#include "../../math/Vector2.h"

// http://www.coranac.com/tonc/text/regobj.htm#tbl-obj-size

namespace GBA
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
			Tall
		};

		enum SizeMode
		{
			Form0,
			Form1,
			Form2,
			Form3
		};

		enum Priority
		{
			Layer0,
			Layer1,
			Layer2,
			Layer3
		};
	}

	class ObjectAttribute
	{
		u16 m_attributeZero;
		u16 m_attributeOne;
		u16 m_attributeTwo;
		u16 padding;

		static const u32 sc_A0_YCOORD_BITINDEX = 0x0;
		static const u32 sc_A0_OBJMODE_BITINDEX = 0x8;
		static const u32 sc_A0_GFXMODE_BITINDEX = 0xA;
		static const u32 sc_A0_MOSAIC_BITINDEX = 0xC;
		static const u32 sc_A0_COLOURMODE_BITINDEX = 0xD;
		static const u32 sc_A0_SHAPE_BITINDEX = 0xE;

		enum Attribute0_Masks
		{
			YCoord		= BITS_INDEXED_U32(8, sc_A0_YCOORD_BITINDEX),// 0-7
			ObjMode		= BITS_INDEXED_U32(2, sc_A0_OBJMODE_BITINDEX),		// 8-9
			GfxMode		= BITS_INDEXED_U32(2, sc_A0_GFXMODE_BITINDEX),				// A-B
			Mosaic		= BITS_INDEXED_U32(1, sc_A0_MOSAIC_BITINDEX),				// C
			ColourMode	= BITS_INDEXED_U32(1, sc_A0_COLOURMODE_BITINDEX),				// D 	
			Shape		= BITS_INDEXED_U32(2, sc_A0_SHAPE_BITINDEX),				// E-F

			A0_Count = 6
		};

		static const u32 sc_A1_XCOORD_BITINDEX = 0x0;
		static const u32 sc_A1_AFFINEINDEX_BITINDEX = 0x9;
		static const u32 sc_A1_HORIZONTALFLIP_BITINDEX = 0xC;
		static const u32 sc_A1_VERTICALFLIP_BITINDEX = 0xD;
		static const u32 sc_A1_SIZEMODE_BITINDEX = 0xE;

		enum Attribute1_Masks
		{
			XCoord			= BITS_INDEXED_U32(9, sc_A1_XCOORD_BITINDEX),	// 0-8
			AffineIndex		= BITS_INDEXED_U32(5, sc_A1_AFFINEINDEX_BITINDEX),	// 9-D: Used only if Affine flag is set
			HorizontalFlip	= BITS_INDEXED_U32(1, sc_A1_HORIZONTALFLIP_BITINDEX),	// C: Used only if Affine flag is clear
			VerticalFlip	= BITS_INDEXED_U32(1, sc_A1_VERTICALFLIP_BITINDEX),	// D: Used only if Affine flag is clear
			SizeMode		= BITS_INDEXED_U32(2, sc_A1_SIZEMODE_BITINDEX),	// E-F

			A1_Count = 5
		};

		static const u32 sc_A2_TILEID_BITINDEX = 0x0;
		static const u32 sc_A2_PRIORITY_BITINDEX = 0xA;
		static const u32 sc_A2_PALETTEBANK_BITINDEX = 0xC;

		enum Attribute2_Masks
		{
			BaseTileIndex	= BITS_INDEXED_U32(10, sc_A2_TILEID_BITINDEX),	// 0-9
			Priority		= BITS_INDEXED_U32(2, sc_A2_PRIORITY_BITINDEX),	// A-B 
			PaletteBank		= BITS_INDEXED_U32(4, sc_A2_PALETTEBANK_BITINDEX),	// C-F: Only used when the colour mode is set to FourBitsPerPixel

			A2_Count = 3
		};

		inline void SetExplicitAttribute(int value, u16 attributeMask, u16& attribute) { attribute = (attribute & ~attributeMask) | (value & attributeMask); }
		inline void SetAttribute(int value, Attribute0_Masks attributeMask) { SetExplicitAttribute(value, attributeMask, m_attributeZero); }
		inline void SetAttribute(int value, Attribute1_Masks attributeMask) { SetExplicitAttribute(value, attributeMask, m_attributeOne); }
		inline void SetAttribute(int value, Attribute2_Masks attributeMask) { SetExplicitAttribute(value, attributeMask, m_attributeTwo); }

	public:
		inline ObjectAttribute() : m_attributeZero(0), m_attributeOne(0), m_attributeTwo(0) {}
		inline void Reset() { m_attributeZero = m_attributeOne = m_attributeTwo = 0; }

		inline void SetPosition(const Vector2& position)	// Top-left of the sprite
		{
			SetAttribute(int(position.x + 0.5f), XCoord);
			SetAttribute(int(position.y + 0.5f), YCoord);
		}

		inline void SetObjectMode(Attributes::ObjectMode objectMode)	{ SetAttribute(SHIFTED_BITMASK((int)objectMode, sc_A0_OBJMODE_BITINDEX), ObjMode); }
		inline void SetGfxMode(Attributes::GfxMode gfxMode)				{ SetAttribute(SHIFTED_BITMASK((int)gfxMode, sc_A0_GFXMODE_BITINDEX), GfxMode); }
		inline void SetColourMode(Attributes::ColourMode colourMode)	{ SetAttribute(SHIFTED_BITMASK((int)colourMode, sc_A0_COLOURMODE_BITINDEX), ColourMode); }
		inline void SetShape(Attributes::Shape shape)					{ SetAttribute(SHIFTED_BITMASK((int)shape, sc_A0_SHAPE_BITINDEX), Shape); }
		inline void SetSizeMode(Attributes::SizeMode sizeMode)			{ SetAttribute(SHIFTED_BITMASK((int)sizeMode, sc_A1_SIZEMODE_BITINDEX), SizeMode); }
		inline void SetPriority(Attributes::Priority layerNum)			{ SetAttribute(SHIFTED_BITMASK((int)layerNum, sc_A2_PRIORITY_BITINDEX), Priority); }
		inline void SetBaseTileIndex(u32 index)							{ SetAttribute(SHIFTED_BITMASK((int)index, sc_A2_TILEID_BITINDEX), BaseTileIndex); }
		inline void SetPaletteIndex(u8 index)							{ SetAttribute(SHIFTED_BITMASK((int)index, sc_A2_PALETTEBANK_BITINDEX), PaletteBank); }

		void SetMosaic(bool enabled);
		void SetFlippedHorizontal(bool flipped);
		void SetFlippedVertical(bool flipped);

	} ALIGN(4);
}
