#pragma once

#include "engine/base/core/stl/DoubleBuffer.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/oam/GBAObjectAffine.h"
#include "engine/gba/graphics/sprite/GBASpriteGraphicsMemoryManager.h"
#include "engine/math/Matrix2x2.h"
#include "GBASDK/ObjectAttributeMemory.h"

namespace GBA
{
	struct ObjectAttribute;

	namespace Gfx
	{
		class Sprite;

		using vObjectAffine = volatile ObjectAffine;

		/* Responsible for loading sprites into vram and drawing them.
		* Tracks which sprites were drawn on the previous frame and the current frame in a double buffer to determine which sprites should be unloaded from vram.
		* Note that fully unloading sprites may resulting in danging pointers in the previous frame buffer, so it's good to leave a frame or manually clear the buffers if we ever need to do this.
		*/
		class OAMManager
		{
		public:
			static constexpr int OBJ_ATTR_COUNT = ARRAY_SIZE(GBA::objectAttributeMemory->attributes);
			static constexpr int OBJ_AFFINE_COUNT = 32;

		private:
			struct OAMSpriteRenderPropertiesSOA
			{
				FixedList<ObjectAttribute, OBJ_ATTR_COUNT> oamProperties;
				FixedList<Sprite*, OBJ_ATTR_COUNT> sprite;
			};

			using ObjAffinePool = Array<vObjectAffine, OBJ_AFFINE_COUNT>;

			typedef FixedList<Sprite*, OBJ_ATTR_COUNT> tSpriteBuffer;

			static ObjAffinePool& s_objectAffinePool;

			Array<bool, OBJ_ATTR_COUNT> m_objAttrEnabledTracker;
			u32 m_objAttrEnabledSearchIndex;

			OAMSpriteRenderPropertiesSOA m_masterSpriteRenderList;
			FixedList<Matrix2x2, OBJ_AFFINE_COUNT> m_affineTransformationList;
			DoubleBuffer<tSpriteBuffer> m_spriteRenderDoubleBuffer;

			SpriteGraphicsMemoryManager m_spriteGraphicsMemoryManager;

			void UnloadUnusedSprites();		// TODO, move this into VDraw and add ability to unload a Sprite from game logic update
			void LoadNewSprites();
			void TransferRenderListIntoMemory();

		public:	

			OAMManager();
			~OAMManager();

			// Executed during VBlank to do the actual sprite drawing. 
			// Needs to be as fast as possible, otherwise visual artifacts may occour if we run past VBlank. 
			void DoMasterRenderIntoMemory();
			void UnloadAll();

			// Use this to draw a sprite to the screen for the current frame.
			// Does not perform sprite screen culling, this is a post-culling step.
			ObjectAttribute* AddToRenderList(Sprite* sprite);

			Matrix2x2* AddToAffineRenderList(u8* out_index);
		};
	}
}
