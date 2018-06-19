#include "GBAOAMManager.h"

#include "engine/base/Macros.h"
#include "engine/engine/engine.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/graphicalassets/sprite/Sprite.h"
#include "engine/graphicalassets/sprite/SpriteManager.h"

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
		SpriteManager* SpriteManager = engine->GetSpriteManager();
	
		for (List<Sprite*>::iterator it = previousBuffer.begin(); it != previousBuffer.end(); ++it)
		{
			Sprite* sprite = (*it);
			if (sprite->IsLoaded() && !currentBuffer.Contains(sprite))
			{
				SpriteManager->Unload(sprite);
			}
		}
	}

	void OAMManager::LoadNewSprites(Engine* engine)
	{
		SpriteManager* SpriteManager = engine->GetSpriteManager();
		List<Sprite*>& buffer = GetCurrentSpriteBuffer();
		for (List<Sprite*>::iterator it = buffer.begin(); it != buffer.end(); ++it)
		{
			Sprite* sprite = (*it);
			if (!sprite->IsLoaded())
			{
				SpriteManager->Load(*sprite);
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
				const Sprite* sprite = spriteProperties.sprite;
				const ObjectAttribute& oamProperties = spriteProperties.oamProperties;

				// Clone current properties. This allows flags like flipping to be more easily decided by the renderer
				oamSpriteHandle.m_attributeZero = oamProperties.m_attributeZero;
				oamSpriteHandle.m_attributeOne = oamProperties.m_attributeOne;
				oamSpriteHandle.m_attributeTwo = oamProperties.m_attributeTwo;

				// Set sprite specific properties
				oamSpriteHandle.SetShape(sprite->GetShape());
				oamSpriteHandle.SetSizeMode(sprite->GetSizeMode());
				oamSpriteHandle.SetPaletteIndex(sprite->GetPaletteIndex());
				oamSpriteHandle.SetTileIndex(sprite->GetTileIndex());				
			}
		}

		m_masterSpriteRenderList.Clear();
	}

	void OAMManager::DoMasterRenderIntoMemory(Engine* engine)
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
		if (!buffer.Contains(sprite))
			buffer.Add(sprite);
	}
}
