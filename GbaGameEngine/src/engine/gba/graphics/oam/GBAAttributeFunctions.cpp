#include "GBAAttributeFunctions.h"
#include "engine/math/Math.h"
#include "engine/graphicalassets/tile/Tile.h"

const u8 c_SIZEMAP_COUNT = GBA::Attributes::ShapeCount * GBA::Attributes::SizeCount;
const Vector2 c_SIZEMAP[c_SIZEMAP_COUNT] = {
	Vector2(1, 1),	Vector2(2, 2),	Vector2(4, 4),	Vector2(8, 8),
	Vector2(2, 1),	Vector2(4, 1),	Vector2(4, 2),	Vector2(8, 4),
	Vector2(1, 2),	Vector2(1, 4),	Vector2(2, 4),	Vector2(4, 8),
};

Vector2 GBA::AttributeFunctions::GetTileSize(Attributes::Shape shape, Attributes::SizeMode sizeMode)
{
	return c_SIZEMAP[shape * Attributes::SizeCount + sizeMode];
}

Vector2 GBA::AttributeFunctions::GetPixelSize(Attributes::Shape shape, Attributes::SizeMode sizeMode)
{
	return GetTileSize(shape, sizeMode) * Tile::PIXELS_SQRROOT_PER_TILE;
}

void GBA::AttributeFunctions::GetSizeAttributesFromTileSize(const Vector2 & tileSize, Attributes::Shape & out_shape, Attributes::SizeMode & out_sizeMode)
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

void GBA::AttributeFunctions::GetSizeAttributesFromPixelSize(const Vector2& pixelSize, Attributes::Shape & out_shape, Attributes::SizeMode & out_sizeMode)
{
	const Vector2 tileSize = pixelSize / Tile::PIXELS_SQRROOT_PER_TILE;
	GetSizeAttributesFromTileSize(tileSize, out_shape, out_sizeMode);
}
