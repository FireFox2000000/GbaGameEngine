#pragma once

#include "engine/base/core/stl/DoubleBuffer.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/sprite/GBASpriteGraphicsMemoryManager.h"
#include "engine/math/Matrix2x2.h"
#include "gbatek/ObjectAttributeMemory.h"

namespace GBATEK
{
	struct ObjectAttribute;
}

namespace GBA
{
	namespace Gfx
	{
		class Sprite;

		/* Responsible for loading sprites into vram and drawing them.
		* Tracks which sprites were drawn on the previous frame and the current frame in a double buffer to determine which sprites should be unloaded from vram.
		* Note that fully unloading sprites may resulting in danging pointers in the previous frame buffer, so it's good to leave a frame or manually clear the buffers if we ever need to do this.
		*/
		class OAMManager
		{
		public:
			static constexpr int OBJ_ATTR_COUNT = ARRAY_SIZE(GBATEK::objectAttributeMemory->attributes);
			static constexpr int OBJ_AFFINE_COUNT = ARRAY_SIZE(GBATEK::objectAttributeMemory->affineAttributes);

		private:
			struct OAMSpriteRenderPropertiesSOA
			{
				FixedList<GBATEK::ObjectAttribute, OBJ_ATTR_COUNT> oamProperties;
				FixedList<Sprite*, OBJ_ATTR_COUNT> sprite;
			};

			typedef FixedList<Sprite*, OBJ_ATTR_COUNT> tSpriteBuffer;

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
			GBATEK::ObjectAttribute* AddToRenderList(Sprite* sprite);

			Matrix2x2* AddToAffineRenderList(u8* out_index);
		};
	}
}
