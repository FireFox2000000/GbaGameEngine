#include "GBAAttributeFunctions.h"
#include "engine/math/Math.h"
#include "engine/gba/graphics/tiles/GBATile.h"

const u8 c_SIZEMAP_COUNT = GBA::Gfx::Attributes::ShapeCount * GBA::Gfx::Attributes::SizeCount;
const GBAAttrFnVector2 c_SIZEMAP[c_SIZEMAP_COUNT] = {
	GBAAttrFnVector2(1, 1),	GBAAttrFnVector2(2, 2),	GBAAttrFnVector2(4, 4),	GBAAttrFnVector2(8, 8),
	GBAAttrFnVector2(2, 1),	GBAAttrFnVector2(4, 1),	GBAAttrFnVector2(4, 2),	GBAAttrFnVector2(8, 4),
	GBAAttrFnVector2(1, 2),	GBAAttrFnVector2(1, 4),	GBAAttrFnVector2(2, 4),	GBAAttrFnVector2(4, 8),
};

namespace GBA
{
	namespace Gfx
	{
		GBAAttrFnVector2 AttributeFunctions::GetTileSize(Attributes::Shape shape, Attributes::SizeMode sizeMode) {
			return c_SIZEMAP[shape * Attributes::SizeCount + sizeMode];
		}

		GBAAttrFnVector2 AttributeFunctions::GetPixelSize(Attributes::Shape shape, Attributes::SizeMode sizeMode) {
			return GetPixelSize(GetTileSize(shape, sizeMode));
		}

		GBAAttrFnVector2 AttributeFunctions::GetPixelSize(const GBAAttrFnVector2& tileSize)
		{
			return tileSize * Gfx::Tile::PIXELS_SQRROOT_PER_TILE;
		}

		void AttributeFunctions::GetSizeAttributesFromTileSize(const GBAAttrFnVector2 & tileSize, Attributes::Shape & out_shape, Attributes::SizeMode & out_sizeMode)
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

		void AttributeFunctions::GetSizeAttributesFromPixelSize(const GBAAttrFnVector2& pixelSize, Attributes::Shape & out_shape, Attributes::SizeMode & out_sizeMode)
		{
			const GBAAttrFnVector2 tileSize = pixelSize / Tile::PIXELS_SQRROOT_PER_TILE;
			GetSizeAttributesFromTileSize(tileSize, out_shape, out_sizeMode);
		}
	}
}
