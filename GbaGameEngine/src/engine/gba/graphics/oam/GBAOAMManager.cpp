#include "GBAOAMManager.h"

#include "engine/base/Macros.h"
#include "engine/engine/engine.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/graphicalassets/sprite/Sprite.h"
#include "engine/graphicalassets/sprite/SpriteLoader.h"

bool PointerComparisonContains(const List<Sprite*>& list, Sprite* sprite)
{
	for (List<Sprite*>::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		if (sprite - *it == 0)
			return true;
	}

	return false;
}

namespace GBA
{
	OAMManager::ObjAttrPool& OAMManager::s_objectAttrPool = *reinterpret_cast<OAMManager::ObjAttrPool*>(OAM_RAM);
	OAMManager::ObjAffinePool& OAMManager::s_objectAffinePool = *reinterpret_cast<OAMManager::ObjAffinePool*>(OAM_RAM);

	OAMManager::OAMManager()
		: m_objAttrEnabledTracker(false)
		, m_objAttrEnabledSearchIndex(0)
		, m_currentSpriteBufferIndex(0)
	{
		for (Array<List<Sprite*>, 2>::iterator it = m_spriteRenderBuffers.begin(); it != m_spriteRenderBuffers.end(); ++it)
		{
			it->Reserve(OBJ_ATTR_COUNT);
		}
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

	List<Sprite*>& OAMManager::GetCurrentSpriteBuffer()
	{
		return m_spriteRenderBuffers[m_currentSpriteBufferIndex];
	}

	List<Sprite*>& OAMManager::GetPreviousSpriteBuffer()
	{
		int previousRenderBufferIndex = m_currentSpriteBufferIndex + 1;
		if (previousRenderBufferIndex > 1)
			previousRenderBufferIndex = 0;

		return m_spriteRenderBuffers[previousRenderBufferIndex];
	}

	void OAMManager::UnloadUnusedSprites(Engine* engine)
	{
		List<Sprite*>& currentBuffer = GetCurrentSpriteBuffer();
		List<Sprite*>& previousBuffer = GetPreviousSpriteBuffer();
		SpriteLoader* spriteLoader = engine->GetSpriteLoader();
	
		for (List<Sprite*>::iterator it = previousBuffer.begin(); it != previousBuffer.end(); ++it)
		{
			Sprite* sprite = (*it);
			if (sprite->IsLoaded() && !PointerComparisonContains(currentBuffer, sprite))
			{
				spriteLoader->Unload(sprite);
			}
		}
	}

	void OAMManager::LoadNewSprites(Engine* engine)
	{
		SpriteLoader* spriteLoader = engine->GetSpriteLoader();
		List<Sprite*>& buffer = GetCurrentSpriteBuffer();
		for (List<Sprite*>::iterator it = buffer.begin(); it != buffer.end(); ++it)
		{
			Sprite* sprite = (*it);
			if (!sprite->IsLoaded())
			{
				spriteLoader->Load(*sprite);
			}
		}
	}

	void OAMManager::TransferRenderListIntoMemory()
	{
		for (u32 i = 0; i < s_objectAttrPool.Count(); ++i)
		{
			vObjectAttribute& oamSpriteHandle = s_objectAttrPool[i];
			oamSpriteHandle.Reset();

			if (i < m_masterSpriteRenderList.Count())
			{
				OAMSpriteRenderProperties& spriteProperties = m_masterSpriteRenderList[i];
				Sprite* sprite = spriteProperties.sprite;

				oamSpriteHandle.SetPaletteIndex(sprite->GetPaletteIndex());
				oamSpriteHandle.SetTileIndex(sprite->GetTileIndex());
				oamSpriteHandle.SetShape(sprite->GetShape());
				oamSpriteHandle.SetSizeMode(sprite->GetSizeMode());
				oamSpriteHandle.SetPosition(spriteProperties.screenPosition);
			}
		}

		m_masterSpriteRenderList.Clear();
	}

	void OAMManager::DoMasterRender(Engine* engine)
	{
		UnloadUnusedSprites(engine);
		LoadNewSprites(engine);
		TransferRenderListIntoMemory();
		FlipRenderBuffer();

		GetCurrentSpriteBuffer().Clear();
	}

	void OAMManager::AddToRenderList(const OAMSpriteRenderProperties& spriteRenderProperties)
	{
		m_masterSpriteRenderList.Add(spriteRenderProperties);

		List<Sprite*>& buffer = GetCurrentSpriteBuffer();
		Sprite* sprite = spriteRenderProperties.sprite;
		if (!PointerComparisonContains(buffer, sprite))
			buffer.Add(sprite);
	}


	// To be removed
	vObjectAttribute * OAMManager::ReserveObject()
	{
		u32 searchIndexStart = m_objAttrEnabledSearchIndex;

		do
		{
			if (!m_objAttrEnabledTracker[m_objAttrEnabledSearchIndex])
			{
				return s_objectAttrPool.At(m_objAttrEnabledSearchIndex++);
			}
			else
			{
				++m_objAttrEnabledSearchIndex;
				if (m_objAttrEnabledSearchIndex >= s_objectAttrPool.Count())
					m_objAttrEnabledSearchIndex = 0;
			}

		} while (m_objAttrEnabledSearchIndex != searchIndexStart);
		
		return NULL;
	}

	void OAMManager::Release(vObjectAttribute * objAttr)
	{
		u32 releaseIndex = s_objectAttrPool.IndexOf(objAttr);
		m_objAttrEnabledTracker[releaseIndex] = false;
		objAttr->Reset();

		// Attempt to reduce affine fragmentation
		if (releaseIndex < m_objAttrEnabledSearchIndex)
			m_objAttrEnabledSearchIndex = releaseIndex;
	}
}
