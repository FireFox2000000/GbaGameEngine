#ifndef PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_OAM_GBAOAMMANAGER_H
#define PRAGMA_ONCE_ENGINE_GBA_GRAPHICS_OAM_GBAOAMMANAGER_H

#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/oam/GBAObjectAttribute.h"
#include "engine/gba/graphics/oam/GBAObjectAffine.h"

class Sprite;
class Engine;

namespace GBA
{
	struct OAMSpriteRenderProperties
	{
		ObjectAttribute oamProperties;
		Sprite* sprite;
	};
}

namespace GBA
{
	typedef volatile ObjectAttribute vObjectAttribute;
	typedef volatile ObjectAffine vObjectAffine;

	class OAMManager
	{
	public:
		static const int OBJ_ATTR_COUNT = 128;
		static const int OBJ_AFFINE_COUNT = 32;
		
	private:
		typedef Array<vObjectAttribute, OBJ_ATTR_COUNT> ObjAttrPool;
		typedef Array<vObjectAffine, OBJ_AFFINE_COUNT> ObjAffinePool;

		typedef FixedList<Sprite*, OBJ_ATTR_COUNT> tSpriteBuffer;

		static ObjAttrPool& s_objectAttrPool;
		static ObjAffinePool& s_objectAffinePool;

		Array<bool, OBJ_ATTR_COUNT> m_objAttrEnabledTracker;
		u32 m_objAttrEnabledSearchIndex;

		FixedList<OAMSpriteRenderProperties, OBJ_ATTR_COUNT> m_masterSpriteRenderList;
		Array<tSpriteBuffer, 2> m_spriteRenderDoubleBuffer;
		int m_currentSpriteBufferIndex;

		void FlipRenderBuffer();
		tSpriteBuffer& GetCurrentSpriteBuffer();
		tSpriteBuffer& GetPreviousSpriteBuffer();

		void UnloadUnusedSprites(Engine* engine);
		void LoadNewSprites(Engine* engine);
		void TransferRenderListIntoMemory();

	public:	

		OAMManager();
		~OAMManager();

		void DoMasterRenderIntoMemory(Engine* engine);
		OAMSpriteRenderProperties* AddToRenderList(Sprite* sprite);
	};
}

#endif
