#pragma once

#include "engine/math/Vector2.h"
#include "gbatek/ObjectAttributeMemory.h"

typedef Vector2<u8> GBAAttrFnVector2;

namespace GBA
{
	namespace Gfx
	{
		namespace AttributeFunctions
		{
			GBAAttrFnVector2 GetTileSize(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode);
			GBAAttrFnVector2 GetPixelSize(GBATEK::ObjectShape shape, GBATEK::ObjectSize sizeMode);
			GBAAttrFnVector2 GetPixelSize(const GBAAttrFnVector2& tileSize);

			void GetSizeAttributesFromTileSize(const GBAAttrFnVector2& tileSize, GBATEK::ObjectShape& out_shape, GBATEK::ObjectSize& out_sizeMode);
			void GetSizeAttributesFromPixelSize(const GBAAttrFnVector2& pixelSize, GBATEK::ObjectShape& out_shape, GBATEK::ObjectSize& out_sizeMode);
		}
	}
}
