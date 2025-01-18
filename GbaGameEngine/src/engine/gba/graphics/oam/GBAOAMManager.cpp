#include "GBAOAMManager.h"
#include "engine/base/Macros.h"
#include "engine/engine/engine.h"
#include "engine/gba/graphics/sprite/GBASprite.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlas.h"

//#define RENDER_PROFILE
#if defined(RENDER_PROFILE)
#include "engine/debug/Profiler.h"
#endif

namespace GBA
{
	namespace Gfx
	{
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
					m_spriteGraphicsMemoryManager.Unload(sprite);
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
					m_spriteGraphicsMemoryManager.Load(*sprite);
				}

				sprite->m_renderData.SetAddedToDrawList(false);
			}
		}

		void OAMManager::TransferRenderListIntoMemory()
		{
			u32 objectCount = m_masterSpriteRenderList.Count();

			// Fast copy ObjectAttributes into memory
			{
				u32 byteCount = sizeof(GBATEK::ObjectAttribute) * objectCount;
				VramSafeMemCopy(GBATEK::objectAttributeMemory->attributes, m_shadowOam.GetData().attributes, objectCount);

				// Remove the rest of the objects by clearing them
				// Even though unused shadow OAM is zeroed out from the previous frame, it's faster 
				// to MemSet 0 than to MemCopy zeroed data
				VramSafeMemSet((u8*)&(GBATEK::objectAttributeMemory->attributes[objectCount]), static_cast<u8>(0), sizeof(GBATEK::objectAttributeMemory->attributes) - byteCount);
			}

			const auto& sprites = m_masterSpriteRenderList;
			for (u32 i = 0; i < objectCount; ++i)
			{
				GBATEK::ObjectAttribute& oamSpriteHandle = GBATEK::objectAttributeMemory->attributes[i];
				const Sprite* sprite = sprites[i];

				// Set just-loaded specific properties
				oamSpriteHandle.palleteBankIndex = sprite->m_atlas->GetPaletteIndex();
				oamSpriteHandle.vramObjectTileIndex = sprite->GetTileIndex();
				oamSpriteHandle.shape = sprite->GetShape();
				oamSpriteHandle.size = sprite->GetSizeMode();
			}

			// Don't use mem-copies here. Will trash ObjectAttributes memory if done so.
			auto& affineAttributes = m_shadowOam.GetData().affineAttributes;
			for (int i = 0; i < m_shadowOam.GetAffineObjectAttributeCount(); ++i)
			{
				GBATEK::ObjectAttributeAffine& oamAffineHandle = GBATEK::objectAttributeMemory->affineAttributes[i];
				oamAffineHandle.paFixedPoint8 = affineAttributes[i].paFixedPoint8;
				oamAffineHandle.pbFixedPoint8 = affineAttributes[i].pbFixedPoint8;
				oamAffineHandle.pcFixedPoint8 = affineAttributes[i].pcFixedPoint8;
				oamAffineHandle.pdFixedPoint8 = affineAttributes[i].pdFixedPoint8;
			}

			m_masterSpriteRenderList.Clear();
			m_shadowOam.Clear();
		}

		void OAMManager::DoMasterRenderIntoMemory()
		{
			{
#ifdef RENDER_PROFILE
				PROFILE_SCOPED_CLOCK_64(Profile_UnloadUnusedSprites);
#endif
				UnloadUnusedSprites();
			}
			{
#ifdef RENDER_PROFILE
				PROFILE_SCOPED_CLOCK_64(Profile_LoadNewSprites);
#endif
				LoadNewSprites();
			}
			{
#ifdef RENDER_PROFILE
				PROFILE_SCOPED_CLOCK_64(Profile_TransferRenderListIntoMemory);
#endif
				TransferRenderListIntoMemory();
			}
			{
#ifdef RENDER_PROFILE
				PROFILE_SCOPED_CLOCK_64(Profile_Flip_And_Clear);
#endif		
				m_spriteRenderDoubleBuffer.Flip();
				m_spriteRenderDoubleBuffer.GetPrimary().Clear();
			}
		}

		void OAMManager::UnloadAll()
		{
			for (Sprite* sprite : m_spriteRenderDoubleBuffer.GetPrimary())
			{
				m_spriteGraphicsMemoryManager.Unload(sprite);
			}

			for (Sprite* sprite : m_spriteRenderDoubleBuffer.GetSecondary())
			{
				m_spriteGraphicsMemoryManager.Unload(sprite);
			}

			m_spriteRenderDoubleBuffer.GetPrimary().Clear();
			m_spriteRenderDoubleBuffer.GetSecondary().Clear();
		}

		GBATEK::ObjectAttribute* OAMManager::AddToRenderList(Sprite* sprite)
		{
			OAMManager::tSpriteBuffer& buffer = m_spriteRenderDoubleBuffer.GetPrimary();

			if (!sprite->m_renderData.IsAddedToDrawList())
			{
				buffer.Add(sprite);
				sprite->m_renderData.SetAddedToDrawList(true);
			}

			// Can't render more than 128, will currently crash if this is exceeded
			GBATEK::ObjectAttribute* properties = m_shadowOam.AllocateObject();

			m_masterSpriteRenderList.Add(sprite);

			return properties;
		}

		GBATEK::ObjectAttributeAffine* OAMManager::AddToAffineRenderList(u8 * out_index)
		{
			*out_index = m_shadowOam.GetAffineObjectAttributeCount();
			GBATEK::ObjectAttributeAffine* affineTransformation = m_shadowOam.AllocateAffineObject();
			return affineTransformation;
		}
	}
}
