#pragma once

#include "GBAObjectAttribute.h"
#include "engine/math/Vector2.h"

typedef Vector2<u8> GBAAttrFnVector2;

namespace GBA
{
	namespace Gfx
	{
		namespace AttributeFunctions
		{
			GBAAttrFnVector2 GetTileSize(Attributes::Shape shape, Attributes::SizeMode sizeMode);
			GBAAttrFnVector2 GetPixelSize(Attributes::Shape shape, Attributes::SizeMode sizeMode);
			GBAAttrFnVector2 GetPixelSize(const GBAAttrFnVector2& tileSize);

			void GetSizeAttributesFromTileSize(const GBAAttrFnVector2& tileSize, Attributes::Shape& out_shape, Attributes::SizeMode& out_sizeMode);
			void GetSizeAttributesFromPixelSize(const GBAAttrFnVector2& pixelSize, Attributes::Shape& out_shape, Attributes::SizeMode& out_sizeMode);
		}
	}
}
