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
			// Fast copy ObjectAttributes into memory
			{
				u32 objByteCount = sizeof(GBATEK::ObjectAttribute) * m_shadowOam.GetObjectAttributeCount();
				u32 affineByteCount = sizeof(GBATEK::ObjectAttributeAffine) * m_shadowOam.GetAffineObjectAttributeCount();
				u32 byteCount = MAX(objByteCount, affineByteCount);
				u8* dest = reinterpret_cast<u8*>(GBATEK::objectAttributeMemory);
				u8* src = reinterpret_cast<u8*>(&m_shadowOam.GetData());

				VramSafeMemCopy(dest, src, byteCount);

				// Remove the rest of the objects by clearing them
				// Even though unused shadow OAM is zeroed out from the previous frame, it's faster 
				// to MemSet 0 than to MemCopy zeroed data
				VramSafeMemSet(dest + byteCount, static_cast<u8>(0), sizeof(*GBATEK::objectAttributeMemory) - byteCount);
			}

			// Setting these properties after the vram copy from sprites directly to io appears 
			// to be faster than applying to shadow oam and copying across with the MemCopy.
			// Not sure why, maybe io ram is faster to access than ewram? Performance lottery shenanigans?
			const auto& sprites = m_masterSpriteRenderList;
			u32 objectCount = m_masterSpriteRenderList.Count();
			for (u32 i = 0; i < objectCount; ++i)
			{
				GBATEK::ObjectAttribute& oamSpriteHandle = GBATEK::objectAttributeMemory->attributes[i];
				const Sprite* sprite = sprites[i];

				// Set just-loaded specific properties
				oamSpriteHandle.palleteBankIndex = sprite->m_atlas->GetPaletteIndex();
				oamSpriteHandle.vramObjectTileIndex = sprite->GetTileIndex();

				// These could be set during AddToRenderList and save cycles in VBlank, however
				// doing so would increase VDraw by even more cycles, eg save ~4500 cycles in VBlank
				// but increases VDraw by ~7500 cycles instead
				oamSpriteHandle.shape = sprite->GetShape();
				oamSpriteHandle.size = sprite->GetSizeMode();
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
