#ifndef PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_OAM_GBAOAMMANAGER_H
#define PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_OAM_GBAOAMMANAGER_H

#include "engine/base/core/stl/Array.h"
#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/oam/GBAObjectAffine.h"

namespace GBA
{
	class OAMManager
	{
	public:
		static const int OBJ_ATTR_COUNT = 128;
		static const int OBJ_AFFINE_COUNT = 32;

	private:
		typedef Array<ObjectAttribute, OBJ_ATTR_COUNT> ObjAttrPool;
		typedef Array<ObjectAffine, OBJ_AFFINE_COUNT> ObjAffinePool;

		static ObjAttrPool& s_objectAttrPool;
		static ObjAffinePool& s_objectAffinePool;

		Array<bool, OBJ_ATTR_COUNT> m_objAttrEnabledTracker;
		u32 m_objAttrEnabledSearchIndex;

		OAMManager();

	public:	
		~OAMManager();

		static OAMManager* GetCurrent()
		{
			static OAMManager instance;
			return &instance;
		}

		ObjectAttribute * ReserveObject();
		void Release(ObjectAttribute* objAttr);
	};
}

#endif
