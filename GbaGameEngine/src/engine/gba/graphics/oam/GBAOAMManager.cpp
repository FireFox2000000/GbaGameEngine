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

	OAMSpriteRenderProperties* OAMManager::AddToRenderList(Sprite* sprite)
	{
		OAMManager::tSpriteBuffer& buffer = GetCurrentSpriteBuffer();
		if (!buffer.Contains(sprite))
			buffer.Add(sprite);

		// Todo, can't render more than 128, will currently crash if this is exceeded
		OAMSpriteRenderProperties* properties = m_masterSpriteRenderList.AddNew();
		properties->sprite = sprite;

		return properties;
	}
}
