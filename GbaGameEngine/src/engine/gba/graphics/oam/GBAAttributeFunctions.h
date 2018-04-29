#ifndef PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_OAM_GBAATTRIBUTEFUNCTIONS_H
#define PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_OAM_GBAATTRIBUTEFUNCTIONS_H

#include "GBAObjectAttribute.h"
#include "engine/math/Vector2.h"

namespace GBA
{
	namespace AttributeFunctions
	{
		Vector2 GetTileSize(Attributes::Shape shape, Attributes::SizeMode sizeMode);
		Vector2 GetPixelSize(Attributes::Shape shape, Attributes::SizeMode sizeMode);
		void GetSizeAttributesFromTileSize(const Vector2& tileSize, Attributes::Shape& out_shape, Attributes::SizeMode& out_sizeMode);
		void GetSizeAttributesFromPixelSize(const Vector2& pixelSize, Attributes::Shape& out_shape, Attributes::SizeMode& out_sizeMode);
	}
}

#endif
