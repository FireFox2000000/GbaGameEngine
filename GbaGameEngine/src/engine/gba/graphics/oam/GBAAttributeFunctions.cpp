#include "GBAAttributeFunctions.h"
#include "engine/math/Math.h"
#include "engine/gba/graphics/tiles/GBATile.h"

constexpr int SHAPE_COUNT = 3;
constexpr int SIZE_COUNT = 4;

const u8 c_SIZEMAP_COUNT = SHAPE_COUNT * SIZE_COUNT;
const GBAAttrFnVector2 c_SIZEMAP[c_SIZEMAP_COUNT] = {
	GBAAttrFnVector2(1, 1),	GBAAttrFnVector2(2, 2),	GBAAttrFnVector2(4, 4),	GBAAttrFnVector2(8, 8),
	GBAAttrFnVector2(2, 1),	GBAAttrFnVector2(4, 1),	GBAAttrFnVector2(4, 2),	GBAAttrFnVector2(8, 4),
	GBAAttrFnVector2(1, 2),	GBAAttrFnVector2(1, 4),	GBAAttrFnVector2(2, 4),	GBAAttrFnVector2(4, 8),
};

namespace GBA
{
	namespace Gfx
	{
		GBAAttrFnVector2 AttributeFunctions::GetTileSize(ObjectShape shape, ObjectSize sizeMode) {
			return c_SIZEMAP[static_cast<int>(shape) * SIZE_COUNT + static_cast<int>(sizeMode)];
		}

		GBAAttrFnVector2 AttributeFunctions::GetPixelSize(ObjectShape shape, ObjectSize sizeMode) {
			return GetPixelSize(GetTileSize(shape, sizeMode));
		}

		GBAAttrFnVector2 AttributeFunctions::GetPixelSize(const GBAAttrFnVector2& tileSize)
		{
			return tileSize * Gfx::Tile::PIXELS_SQRROOT_PER_TILE;
		}

		void AttributeFunctions::GetSizeAttributesFromTileSize(const GBAAttrFnVector2 & tileSize, ObjectShape& out_shape, ObjectSize& out_sizeMode)
		{
			for (u8 i = 0; i < c_SIZEMAP_COUNT; ++i)
			{
				if (c_SIZEMAP[i] == tileSize)
				{
					out_shape = ObjectShape(i / SIZE_COUNT);
					out_sizeMode = ObjectSize(i % SIZE_COUNT);
					return;
				}
			}
		}

		void AttributeFunctions::GetSizeAttributesFromPixelSize(const GBAAttrFnVector2& pixelSize, ObjectShape& out_shape, ObjectSize& out_sizeMode)
		{
			const GBAAttrFnVector2 tileSize = pixelSize / Tile::PIXELS_SQRROOT_PER_TILE;
			GetSizeAttributesFromTileSize(tileSize, out_shape, out_sizeMode);
		}
	}
}
