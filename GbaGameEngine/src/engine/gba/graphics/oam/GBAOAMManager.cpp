#include "GBAOAMManager.h"
#include "engine/base/Macros.h"
#include "engine/engine/engine.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/graphicalassets/sprite/Sprite.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"
#include "engine/gba/registers/clock/GBATimer.h"
#include "engine/debug/DebugLog.h"

//#define RENDER_PROFILE

namespace GBA
{
	OAMManager::ObjAttrPool& OAMManager::s_objectAttrPool = *reinterpret_cast<OAMManager::ObjAttrPool*>(OAM_RAM);
	OAMManager::ObjAffinePool& OAMManager::s_objectAffinePool = *reinterpret_cast<OAMManager::ObjAffinePool*>(OAM_RAM);

	OAMManager::OAMManager()
		: m_objAttrEnabledTracker(false)
		, m_objAttrEnabledSearchIndex(0)
		, m_currentSpriteBufferIndex(0)
	{
	}

	OAMManager::~OAMManager()
	{
	}

	void OAMManager::FlipRenderBuffer()
	{
		++m_currentSpriteBufferIndex;
		if (m_currentSpriteBufferIndex > 1)
			m_currentSpriteBufferIndex = 0;
	}

	OAMManager::tSpriteBuffer& OAMManager::GetCurrentSpriteBuffer()
	{
		return m_spriteRenderDoubleBuffer[m_currentSpriteBufferIndex];
	}

	OAMManager::tSpriteBuffer& OAMManager::GetPreviousSpriteBuffer()
	{
		int previousRenderBufferIndex = m_currentSpriteBufferIndex + 1;
		if (previousRenderBufferIndex > 1)
			previousRenderBufferIndex = 0;

		return m_spriteRenderDoubleBuffer[previousRenderBufferIndex];
	}

	void OAMManager::UnloadUnusedSprites(Engine* engine)
	{
		tSpriteBuffer& currentBuffer = GetCurrentSpriteBuffer();
		tSpriteBuffer& previousBuffer = GetPreviousSpriteBuffer();
		SpriteManager* spriteManager = engine->EditComponent<SpriteManager>();
	
		for (Sprite* sprite : previousBuffer)
		{
			if (sprite->IsLoaded() && !currentBuffer.Contains(sprite))
			{
				spriteManager->Unload(sprite);
			}
		}
	}

	void OAMManager::LoadNewSprites(Engine* engine)
	{
		SpriteManager* spriteManager = engine->EditComponent<SpriteManager>();
		tSpriteBuffer& buffer = GetCurrentSpriteBuffer();
		for (Sprite* sprite : buffer)
		{
			if (!sprite->IsLoaded())
			{
				spriteManager->Load(*sprite);
			}
		}
	}

	void OAMManager::TransferRenderListIntoMemory()
	{
		u32 objectCount = m_masterSpriteRenderList.oamProperties.Count();

		// Fast copy ObjectAttributes into memory
		{
			int byteCount = sizeof(ObjectAttribute) * objectCount;

			MemCopy(&m_masterSpriteRenderList.oamProperties, (void*)(OAM_RAM), byteCount);

			// Remove the rest of the objects by clearing them
			memset((void*)(OAM_RAM + byteCount), 0, sizeof(s_objectAttrPool) - byteCount);
		}

		for (u32 i = 0; i < objectCount; ++i)
		{
			vObjectAttribute& oamSpriteHandle = s_objectAttrPool[i];
			const Sprite* sprite = m_masterSpriteRenderList.sprite[i];

			// Set just-loaded specific properties
			oamSpriteHandle.SetPaletteIndex(sprite->GetPaletteIndex());
			oamSpriteHandle.SetTileIndex(sprite->GetTileIndex());
			oamSpriteHandle.SetShape(sprite->GetShape());
			oamSpriteHandle.SetSizeMode(sprite->GetSizeMode());
		}

		m_masterSpriteRenderList.oamProperties.Clear();
		m_masterSpriteRenderList.sprite.Clear();
	}

	void OAMManager::DoMasterRenderIntoMemory(Engine* engine)
	{
#ifdef RENDER_PROFILE
		GBA::Timer profilerClock(2);
		profilerClock.SetFrequency(GBA::Timer::Cycle_256);

		profilerClock.SetActive(true);
#endif
		UnloadUnusedSprites(engine);
#ifdef RENDER_PROFILE
		DEBUG_LOGFORMAT("[Profile UnloadUnusedSprites] = %d", profilerClock.GetCurrentTimerCount());
		profilerClock.SetActive(false);

		profilerClock.SetActive(true);
#endif
		LoadNewSprites(engine);
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
		FlipRenderBuffer();

		GetCurrentSpriteBuffer().Clear();
#ifdef RENDER_PROFILE
		DEBUG_LOGFORMAT("[Profile Flip + Clear] = %d", profilerClock.GetCurrentTimerCount());
		profilerClock.SetActive(false);
#endif
	}

	ObjectAttribute* OAMManager::AddToRenderList(Sprite* sprite)
	{
		OAMManager::tSpriteBuffer& buffer = GetCurrentSpriteBuffer();
		if (!buffer.Contains(sprite))
			buffer.Add(sprite);

		// Todo, can't render more than 128, will currently crash if this is exceeded
		ObjectAttribute* properties = m_masterSpriteRenderList.oamProperties.AddNew();
		m_masterSpriteRenderList.sprite.Add(sprite);

		return properties;
	}
}
