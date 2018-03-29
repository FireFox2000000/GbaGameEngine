#pragma once
#include "../../Typedefs.h"
#include "../../Macros.h"
#include "../../math/Vector2.h"

namespace GBA
{
	namespace Attributes
	{
		enum class ObjectMode
		{
			Normal,					
			Affine,					
			Hidden,					
			AffineDoubleRendering	
		};

		enum class GfxMode
		{
			Normal,		
			AlphaBlend,	
			WindowMask	
		};

		enum class ColourMode
		{
			FourBitsPerPixel,
			EightBitsPerPixel
		};

		enum class Shape
		{
			Square,
			Wide,	
			Tall,	
		};

		enum class SizeMode
		{
			Form0,
			Form1,
			Form2,
			Form3
		};

		enum class Priority
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

		static const int sc_A0_YCOORD_BITINDEX = 0x0;
		static const int sc_A0_OBJMODE_BITINDEX = 0x8;
		static const int sc_A0_GFXMODE_BITINDEX = 0xA;
		static const int sc_A0_MOSAIC_BITINDEX = 0xC;
		static const int sc_A0_COLOURMODE_BITINDEX = 0xD;
		static const int sc_A0_SHAPE_BITINDEX = 0xE;

		enum Attribute0_Masks : u16
		{
			YCoord		= SHIFTED_BITMASK(0xFF, sc_A0_YCOORD_BITINDEX),		// 0-7
			ObjMode		= SHIFTED_BITMASK(0x3, sc_A0_OBJMODE_BITINDEX),		// 8-9
			GfxMode		= SHIFTED_BITMASK(0x3, sc_A0_GFXMODE_BITINDEX),				// A-B
			Mosaic		= SHIFTED_BITMASK(0x1, sc_A0_MOSAIC_BITINDEX),				// C
			ColourMode	= SHIFTED_BITMASK(0x1, sc_A0_COLOURMODE_BITINDEX),				// D 	
			Shape		= SHIFTED_BITMASK(0x3, sc_A0_SHAPE_BITINDEX),				// E-F

			A0_Count = 6
		};

		static const int sc_A1_XCOORD_BITINDEX = 0x0;
		static const int sc_A1_AFFINEINDEX_BITINDEX = 0x9;
		static const int sc_A1_HORIZONTALFLIP_BITINDEX = 0xC;
		static const int sc_A1_VERTICALFLIP_BITINDEX = 0xD;
		static const int sc_A1_SIZEMODE_BITINDEX = 0xE;

		enum Attribute1_Masks : u16
		{
			XCoord			= SHIFTED_BITMASK(0x1FF, sc_A1_XCOORD_BITINDEX),	// 0-8
			AffineIndex		= SHIFTED_BITMASK(0x1F, sc_A1_AFFINEINDEX_BITINDEX),	// 9-D: Used only if Affine flag is set
			HorizontalFlip	= SHIFTED_BITMASK(0x1, sc_A1_HORIZONTALFLIP_BITINDEX),	// C: Used only if Affine flag is clear
			VerticalFlip	= SHIFTED_BITMASK(0x1, sc_A1_VERTICALFLIP_BITINDEX),	// D: Used only if Affine flag is clear
			SizeMode		= SHIFTED_BITMASK(0x3, sc_A1_SIZEMODE_BITINDEX),	// E-F

			A1_Count = 5
		};

		static const int sc_A2_TILEID_BITINDEX = 0x0;
		static const int sc_A2_PRIORITY_BITINDEX = 0xA;
		static const int sc_A2_PALETTEBANK_BITINDEX = 0xC;

		enum Attribute2_Masks : u16
		{
			TileId			= SHIFTED_BITMASK(0x3FF, sc_A2_TILEID_BITINDEX),	// 0-9
			Priority		= SHIFTED_BITMASK(0x3, sc_A2_PRIORITY_BITINDEX),	// A-B 
			PaletteBank		= SHIFTED_BITMASK(0xF, sc_A2_PALETTEBANK_BITINDEX),	// C-F: Only used when the colour mode is set to FourBitsPerPixel

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

		void SetMosaic(bool enabled);
		void SetFlippedHorizontal(bool flipped);
		void SetFlippedVertical(bool flipped);

	} ALIGN(4);
}