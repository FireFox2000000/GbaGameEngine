#include "GBAOAMManager.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/base/Macros.h"

namespace GBA
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

	ObjectAttribute * OAMManager::ReserveObject()
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

	void OAMManager::Release(ObjectAttribute * objAttr)
	{
		u32 releaseIndex = s_objectAttrPool.IndexOf(objAttr);
		m_objAttrEnabledTracker[releaseIndex] = false;
		objAttr->Reset();

		// Attempt to reduce affine fragmentation
		if (releaseIndex < m_objAttrEnabledSearchIndex)
			m_objAttrEnabledSearchIndex = releaseIndex;
	}
}
