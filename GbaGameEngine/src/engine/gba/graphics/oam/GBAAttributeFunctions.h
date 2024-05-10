#pragma once

#include "engine/math/Vector2.h"
#include "GBASDK/ObjectAttributeMemory.h"

typedef Vector2<u8> GBAAttrFnVector2;

namespace GBA
{
	namespace Gfx
	{
		namespace AttributeFunctions
		{
			GBAAttrFnVector2 GetTileSize(ObjectShape shape, ObjectSize sizeMode);
			GBAAttrFnVector2 GetPixelSize(ObjectShape shape, ObjectSize sizeMode);
			GBAAttrFnVector2 GetPixelSize(const GBAAttrFnVector2& tileSize);

			void GetSizeAttributesFromTileSize(const GBAAttrFnVector2& tileSize, ObjectShape& out_shape, ObjectSize& out_sizeMode);
			void GetSizeAttributesFromPixelSize(const GBAAttrFnVector2& pixelSize, ObjectShape& out_shape, ObjectSize& out_sizeMode);
		}
	}
}
