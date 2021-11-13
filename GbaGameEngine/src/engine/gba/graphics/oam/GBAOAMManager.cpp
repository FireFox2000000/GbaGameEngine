#include "GBAOAMManager.h"
#include "engine/base/Macros.h"
#include "engine/engine/engine.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/gba/graphics/sprite/GBASprite.h"
#include "engine/gba/registers/clock/GBATimer.h"

//#define RENDER_PROFILE

namespace GBA
{
	namespace Gfx
	{
		OAMManager::ObjAttrPool& OAMManager::s_objectAttrPool = *reinterpret_cast<OAMManager::ObjAttrPool*>(OAM_RAM);
		OAMManager::ObjAffinePool& OAMManager::s_objectAffinePool = *reinterpret_cast<OAMManager::ObjAffinePool*>(OAM_RAM);

		OAMManager::OAMManager()
			: m_objAttrEnabledTracker(false)
			, m_objAttrEnabledSearchIndex(0)
		{
		}

		OAMManager::~OAMManager()
		{
		}

		void OAMManager::UnloadUnusedSprites()
		{
			tSpriteBuffer& previousBuffer = m_spriteRenderDoubleBuffer.GetSecondary();

			for (Sprite* sprite : previousBuffer)
			{
				if (sprite->IsLoaded() && !sprite->m_renderData.IsAddedToDrawList())
				{
					m_spriteManager.Unload(sprite);
				}
			}
		}

		void OAMManager::LoadNewSprites()
		{
			tSpriteBuffer& buffer = m_spriteRenderDoubleBuffer.GetPrimary();
			for (Sprite* sprite : buffer)
			{
				if (!sprite->IsLoaded())
				{
					m_spriteManager.Load(*sprite);
				}

				sprite->m_renderData.SetAddedToDrawList(false);
			}
		}

		void OAMManager::TransferRenderListIntoMemory()
		{
			u32 objectCount = m_masterSpriteRenderList.oamProperties.Count();

			// Fast copy ObjectAttributes into memory
			{
				u32 byteCount = sizeof(ObjectAttribute) * objectCount;
				VramSafeMemCopy(m_masterSpriteRenderList.oamProperties.GetContainer(), (void*)&s_objectAttrPool, byteCount);

				// Remove the rest of the objects by clearing them
				VramSafeMemSet((void*)&s_objectAttrPool[objectCount], (u8)0, sizeof(s_objectAttrPool) - byteCount);
			}

			const auto& sprites = m_masterSpriteRenderList.sprite;
			for (u32 i = 0; i < objectCount; ++i)
			{
				vObjectAttribute& oamSpriteHandle = s_objectAttrPool[i];
				const Sprite* sprite = sprites[i];

				// Set just-loaded specific properties
				oamSpriteHandle.SetPaletteIndex(sprite->GetPaletteIndex());
				oamSpriteHandle.SetTileIndex(sprite->GetTileIndex());
				oamSpriteHandle.SetShape(sprite->GetShape());
				oamSpriteHandle.SetSizeMode(sprite->GetSizeMode());
			}

			const auto& affineTransformations = m_masterSpriteRenderList.affineTransformation;
			for (u32 i = 0; i < affineTransformations.Count(); ++i)
			{
				vObjectAffine& oamAffineHandle = s_objectAffinePool[i];
				oamAffineHandle.SetTransformation(affineTransformations[i]);
			}

			m_masterSpriteRenderList.oamProperties.Clear();
			m_masterSpriteRenderList.sprite.Clear();
			m_masterSpriteRenderList.affineTransformation.Clear();
		}

		void OAMManager::DoMasterRenderIntoMemory()
		{
#ifdef RENDER_PROFILE
			auto& profilerClock = GBA::Timers::GetTimer(GBA::Timers::Profile);
			profilerClock.SetFrequency(GBA::Timers::Cycle_64);

			profilerClock.SetActive(true);
#endif
			UnloadUnusedSprites();
#ifdef RENDER_PROFILE
			DEBUG_LOGFORMAT("[Profile UnloadUnusedSprites] = %d", profilerClock.GetCurrentTimerCount());
			profilerClock.SetActive(false);

			profilerClock.SetActive(true);
#endif
			LoadNewSprites();
#ifdef RENDER_PROFILE
			DEBUG_LOGFORMAT("[Profile LoadNewSprites] = %d", profilerClock.GetCurrentTimerCount());
			profilerClock.SetActive(false);

			profilerClock.SetActive(true);
#endif
			TransferRenderListIntoMemory();
#ifdef RENDER_PROFILE
			DEBUG_LOGFORMAT("[Profile TransferRenderListIntoMemory] = %d", profilerClock.GetCurrentTimerCount());
			profilerClock.SetActive(false);

			profilerClock.SetActive(true);
#endif
			m_spriteRenderDoubleBuffer.Flip();

			m_spriteRenderDoubleBuffer.GetPrimary().Clear();
#ifdef RENDER_PROFILE
			DEBUG_LOGFORMAT("[Profile Flip + Clear] = %d", profilerClock.GetCurrentTimerCount());
			profilerClock.SetActive(false);
#endif
		}

		void OAMManager::Clear()
		{
			m_spriteRenderDoubleBuffer.GetPrimary().Clear();
			m_spriteRenderDoubleBuffer.GetSecondary().Clear();
		}

		ObjectAttribute* OAMManager::AddToRenderList(Sprite* sprite)
		{
			OAMManager::tSpriteBuffer& buffer = m_spriteRenderDoubleBuffer.GetPrimary();

			if (!sprite->m_renderData.IsAddedToDrawList())
			{
				buffer.Add(sprite);
				sprite->m_renderData.SetAddedToDrawList(true);
			}

			DEBUG_ASSERTMSG(m_masterSpriteRenderList.oamProperties.Count() < OBJ_ATTR_COUNT, "OUT OF OAM MEMORY");

			// Todo, can't render more than 128, will currently crash if this is exceeded
			ObjectAttribute* properties = m_masterSpriteRenderList.oamProperties.AddNew();
			m_masterSpriteRenderList.sprite.Add(sprite);

			return properties;
		}

		Matrix2x2* OAMManager::AddToAffineRenderList(u8 * out_index)
		{
			DEBUG_ASSERTMSG(m_masterSpriteRenderList.affineTransformation.Count() < OBJ_AFFINE_COUNT, "OUT OF OAM AFFINE MEMORY");

			*out_index = m_masterSpriteRenderList.affineTransformation.Count();

			Matrix2x2* affineTransformation = m_masterSpriteRenderList.affineTransformation.AddNew();
			return affineTransformation;
		}
	}
}
