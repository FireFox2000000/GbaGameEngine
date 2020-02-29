#pragma once

#include "engine/base/core/stl/DoubleBuffer.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/oam/GBAObjectAffine.h"
#include "engine/gba/graphics/sprite/GBASpriteManager.h"

namespace GBA
{
	namespace Gfx
	{
		class Sprite;

		typedef volatile ObjectAttribute vObjectAttribute;
		typedef volatile ObjectAffine vObjectAffine;

		class OAMManager
		{
		public:
			static const int OBJ_ATTR_COUNT = 128;
			static const int OBJ_AFFINE_COUNT = 32;
		
		private:
			struct OAMSpriteRenderPropertiesSOA
			{
				FixedList<ObjectAttribute, OBJ_ATTR_COUNT> oamProperties;
				FixedList<Sprite*, OBJ_ATTR_COUNT> sprite;
			};

			typedef Array<vObjectAttribute, OBJ_ATTR_COUNT> ObjAttrPool;
			typedef Array<vObjectAffine, OBJ_AFFINE_COUNT> ObjAffinePool;

			typedef FixedList<Sprite*, OBJ_ATTR_COUNT> tSpriteBuffer;

			static ObjAttrPool& s_objectAttrPool;
			static ObjAffinePool& s_objectAffinePool;

			Array<bool, OBJ_ATTR_COUNT> m_objAttrEnabledTracker;
			u32 m_objAttrEnabledSearchIndex;

			OAMSpriteRenderPropertiesSOA m_masterSpriteRenderList;
			DoubleBuffer<tSpriteBuffer> m_spriteRenderDoubleBuffer;

			SpriteManager m_spriteManager;

			void UnloadUnusedSprites();
			void LoadNewSprites();
			void TransferRenderListIntoMemory();

		public:	

			OAMManager();
			~OAMManager();

			void DoMasterRenderIntoMemory();
			ObjectAttribute* AddToRenderList(Sprite* sprite);
		};
	}
}
