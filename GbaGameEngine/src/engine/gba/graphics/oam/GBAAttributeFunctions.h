#pragma once

#include "GBAObjectAttribute.h"
#include "engine/math/Vector2.h"
#include "engine/graphicalassets/tile/Tile.h"

typedef Vector2<int> GBAAttrFnVector2;

namespace GBA
{
	namespace AttributeFunctions
	{
		const u8 c_SIZEMAP_COUNT = GBA::Attributes::ShapeCount * GBA::Attributes::SizeCount;
		const GBAAttrFnVector2 c_SIZEMAP[c_SIZEMAP_COUNT] = {
			GBAAttrFnVector2(1, 1),	GBAAttrFnVector2(2, 2),	GBAAttrFnVector2(4, 4),	GBAAttrFnVector2(8, 8),
			GBAAttrFnVector2(2, 1),	GBAAttrFnVector2(4, 1),	GBAAttrFnVector2(4, 2),	GBAAttrFnVector2(8, 4),
			GBAAttrFnVector2(1, 2),	GBAAttrFnVector2(1, 4),	GBAAttrFnVector2(2, 4),	GBAAttrFnVector2(4, 8),
		};

		inline GBAAttrFnVector2 GetTileSize(Attributes::Shape shape, Attributes::SizeMode sizeMode) { 
			return c_SIZEMAP[shape * Attributes::SizeCount + sizeMode];
		}
		inline GBAAttrFnVector2 GetPixelSize(Attributes::Shape shape, Attributes::SizeMode sizeMode) {
			return GetTileSize(shape, sizeMode) * Tile::PIXELS_SQRROOT_PER_TILE;
		}
		void GetSizeAttributesFromTileSize(const GBAAttrFnVector2& tileSize, Attributes::Shape& out_shape, Attributes::SizeMode& out_sizeMode);
		void GetSizeAttributesFromPixelSize(const GBAAttrFnVector2& pixelSize, Attributes::Shape& out_shape, Attributes::SizeMode& out_sizeMode);
	}
}
