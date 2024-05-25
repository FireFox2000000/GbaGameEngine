#include "GBAOAMManager.h"
#include "engine/base/Macros.h"
#include "engine/engine/engine.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/gba/graphics/sprite/GBASprite.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"

//#define RENDER_PROFILE
#if defined(RENDER_PROFILE)
#include "engine/gba/config/GBATimerId.h"
#include "GBASDK/Timers.h"
#endif

constexpr GBA::ObjectAttribute MakeDefaultAttribute()
{
	GBA::ObjectAttribute attr{};

	attr.screenPosY = 0;
	attr.objectMode = GBA::ObjectMode::Normal;
	attr.gfxMode = GBA::GfxMode::Normal;
	attr.mosaic = GBA::MosaicState::Off;
	attr.colourMode = GBA::ColourMode::FourBitsPerPixel;
	attr.shape = GBA::ObjectShape::Square;

	attr.screenPosX = 0;
	attr.flipHorizontal = GBA::ObjectFlippedState::Normal;
	attr.flipVertical = GBA::ObjectFlippedState::Normal;
	attr.size = GBA::ObjectSize::Smallest;

	attr.baseTileIndex = 0;
	attr.priority = 0;
	attr.palleteBankIndex = 0;

	return attr;
}

constexpr GBA::ObjectAttribute DEFAULT_ATTR = MakeDefaultAttribute();

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
			u32 objectCount = m_masterSpriteRenderList.oamProperties.Count();

			// Fast copy ObjectAttributes into memory
			{
				u32 byteCount = sizeof(ObjectAttribute) * objectCount;
				VramSafeMemCopy(GBA::objectAttributeMemory, m_masterSpriteRenderList.oamProperties.GetContainer(), byteCount);

				// Remove the rest of the objects by clearing them
				VramSafeMemSet((u8*)&(GBA::objectAttributeMemory->attributes[objectCount]), static_cast<u8>(0), sizeof(GBA::objectAttributeMemory->attributes) - byteCount);
			}

			const auto& sprites = m_masterSpriteRenderList.sprite;
			for (u32 i = 0; i < objectCount; ++i)
			{
				GBA::ObjectAttribute& oamSpriteHandle = GBA::objectAttributeMemory->attributes[i];
				const Sprite* sprite = sprites[i];

				// Set just-loaded specific properties
				oamSpriteHandle.palleteBankIndex = sprite->m_atlus->GetPaletteIndex();
				oamSpriteHandle.baseTileIndex = sprite->GetTileIndex();
				oamSpriteHandle.shape = sprite->GetShape();
				oamSpriteHandle.size = sprite->GetSizeMode();
			}

			// Don't use mem-copies here. Will trash ObjectAttributes memory if done so. 
			for (u32 i = 0; i < m_affineTransformationList.Count(); ++i)
			{
				ObjectAttributeAffine& oamAffineHandle = GBA::objectAttributeMemory->affineAttributes[i];
				oamAffineHandle.paFixedPoint8 = m_affineTransformationList[i].a.GetStorage();
				oamAffineHandle.pbFixedPoint8 = m_affineTransformationList[i].b.GetStorage();
				oamAffineHandle.pcFixedPoint8 = m_affineTransformationList[i].c.GetStorage();
				oamAffineHandle.pdFixedPoint8 = m_affineTransformationList[i].d.GetStorage();
			}

			m_masterSpriteRenderList.oamProperties.Clear();
			m_masterSpriteRenderList.sprite.Clear();
			m_affineTransformationList.Clear();
		}

		void OAMManager::DoMasterRenderIntoMemory()
		{
#ifdef RENDER_PROFILE
			TimerControl& profilerClock = GBA::ioRegisterTimers->at(GBATimerId::Profile);
			profilerClock.frequency = GBA::ClockFrequency::Cycle_64;

			profilerClock.isEnabled = true;
#endif
			UnloadUnusedSprites();
#ifdef RENDER_PROFILE
			DEBUG_LOGFORMAT("[Profile UnloadUnusedSprites] = %d", profilerClock.GetCurrentCount());
			profilerClock.isEnabled = false;

			profilerClock.isEnabled = true;
#endif
			LoadNewSprites();
#ifdef RENDER_PROFILE
			DEBUG_LOGFORMAT("[Profile LoadNewSprites] = %d", profilerClock.GetCurrentCount());
			profilerClock.isEnabled = false;

			profilerClock.isEnabled = true;
#endif
			TransferRenderListIntoMemory();
#ifdef RENDER_PROFILE
			DEBUG_LOGFORMAT("[Profile TransferRenderListIntoMemory] = %d", profilerClock.GetCurrentCount());
			profilerClock.isEnabled = false;

			profilerClock.isEnabled = true;
#endif
			m_spriteRenderDoubleBuffer.Flip();

			m_spriteRenderDoubleBuffer.GetPrimary().Clear();
#ifdef RENDER_PROFILE
			DEBUG_LOGFORMAT("[Profile Flip + Clear] = %d", profilerClock.GetCurrentCount());
			profilerClock.isEnabled = false; 
#endif
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

		ObjectAttribute* OAMManager::AddToRenderList(Sprite* sprite)
		{
			OAMManager::tSpriteBuffer& buffer = m_spriteRenderDoubleBuffer.GetPrimary();

			if (!sprite->m_renderData.IsAddedToDrawList())
			{
				buffer.Add(sprite);
				sprite->m_renderData.SetAddedToDrawList(true);
			}

			DEBUG_ASSERTMSG(m_masterSpriteRenderList.oamProperties.Count() < OBJ_ATTR_COUNT, "OUT OF OAM MEMORY");

			// Can't render more than 128, will currently crash if this is exceeded
			ObjectAttribute* properties = m_masterSpriteRenderList.oamProperties.AddNew(DEFAULT_ATTR);
			m_masterSpriteRenderList.sprite.Add(sprite);

			return properties;
		}

		Matrix2x2* OAMManager::AddToAffineRenderList(u8 * out_index)
		{
			DEBUG_ASSERTMSG(m_affineTransformationList.Count() < OBJ_AFFINE_COUNT, "OUT OF OAM AFFINE MEMORY");

			*out_index = m_affineTransformationList.Count();

			Matrix2x2* affineTransformation = m_affineTransformationList.AddNew();
			return affineTransformation;
		}
	}
}
