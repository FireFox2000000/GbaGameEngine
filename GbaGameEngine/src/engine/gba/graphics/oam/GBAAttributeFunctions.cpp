#include "GBAAttributeFunctions.h"
#include "engine/math/Math.h"
#include "engine/gba/graphics/tiles/GBATile.h"

constexpr int SHAPE_COUNT = 3;
constexpr int SIZE_COUNT = 4;

const u8 c_SIZEMAP_COUNT = SHAPE_COUNT * SIZE_COUNT;
const Vector2<u8> c_SIZEMAP[c_SIZEMAP_COUNT] = {
	{1, 1},	{2, 2},	{4, 4},	{8, 8},
	{2, 1},	{4, 1},	{4, 2},	{8, 4},
	{1, 2},	{1, 4},	{2, 4},	{4, 8},
};

namespace GBA
{
	namespace Gfx
	{
		Vector2<u8> AttributeFunctions::GetTileSize(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode) {
			return c_SIZEMAP[static_cast<int>(shape) * SIZE_COUNT + static_cast<int>(sizeMode)];
		}

		Vector2<u8> AttributeFunctions::GetPixelSize(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode) {
			return GetPixelSize(GetTileSize(shape, sizeMode));
		}

		Vector2<u8> AttributeFunctions::GetPixelSize(const Vector2<u8>& tileSize)
		{
			return tileSize * Gfx::Tile::PIXELS_SQRROOT_PER_TILE;
		}

		void AttributeFunctions::GetSizeAttributesFromTileSize(const Vector2<u8>& tileSize, GBATEK::ObjectShape& out_shape, GBATEK::ObjectSize& out_sizeMode)
		{
			for (u8 i = 0; i < c_SIZEMAP_COUNT; ++i)
			{
				if (c_SIZEMAP[i] == tileSize)
				{
					out_shape = GBATEK::ObjectShape(i / SIZE_COUNT);
					out_sizeMode = GBATEK::ObjectSize(i % SIZE_COUNT);
					return;
				}
			}
		}

		void AttributeFunctions::GetSizeAttributesFromPixelSize(const Vector2<u8>& pixelSize, GBATEK::ObjectShape& out_shape, GBATEK::ObjectSize& out_sizeMode)
		{
			const Vector2<u8> tileSize = pixelSize / Tile::PIXELS_SQRROOT_PER_TILE;
			GetSizeAttributesFromTileSize(tileSize, out_shape, out_sizeMode);
		}
	}
}
