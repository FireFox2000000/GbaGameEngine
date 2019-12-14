#include "GBAAttributeFunctions.h"
#include "engine/math/Math.h"

void GBA::AttributeFunctions::GetSizeAttributesFromTileSize(const GBAAttrFnVector2 & tileSize, Attributes::Shape & out_shape, Attributes::SizeMode & out_sizeMode)
{
	for (u8 i = 0; i < c_SIZEMAP_COUNT; ++i)
	{
		if (c_SIZEMAP[i] == tileSize)
		{
			out_shape = Attributes::Shape(i / Attributes::SizeCount);
			out_sizeMode = Attributes::SizeMode(i % Attributes::SizeCount);
			return;
		}
	}
}

void GBA::AttributeFunctions::GetSizeAttributesFromPixelSize(const GBAAttrFnVector2& pixelSize, Attributes::Shape & out_shape, Attributes::SizeMode & out_sizeMode)
{
	const GBAAttrFnVector2 tileSize = pixelSize / Tile::PIXELS_SQRROOT_PER_TILE;
	GetSizeAttributesFromTileSize(tileSize, out_shape, out_sizeMode);
}
