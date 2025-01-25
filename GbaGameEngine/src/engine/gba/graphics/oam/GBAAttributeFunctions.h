#pragma once

#include "engine/math/Vector2.h"
#include "gbatek/ObjectAttributeMemory.h"

namespace GBA
{
	namespace Gfx
	{
		namespace AttributeFunctions
		{
			Vector2<u8> GetTileSize(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode);
			Vector2<u8> GetPixelSize(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode);
			Vector2<u8> GetPixelSize(const Vector2<u8>& tileSize);

			void GetSizeAttributesFromTileSize(const Vector2<u8>& tileSize, GBATEK::ObjectShape& out_shape, GBATEK::ObjectSize& out_sizeMode);
			void GetSizeAttributesFromPixelSize(const Vector2<u8>& pixelSize, GBATEK::ObjectShape& out_shape, GBATEK::ObjectSize& out_sizeMode);
		}
	}
}
