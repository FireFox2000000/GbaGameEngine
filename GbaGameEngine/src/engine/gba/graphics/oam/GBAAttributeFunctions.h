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
	}
}

#endif
