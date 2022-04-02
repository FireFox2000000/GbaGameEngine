#include "EntityComponentManager.h"
#include "engine/base/Macros.h"

void ECS::EntityComponentManager::InternalFinaliseDestroy()
{
	for (Entity e : m_entitesToDestroyQueue)
	{
		DEBUG_LOGFORMAT("Destroying entity %d", e);
		m_registry.destroy(e);
	}

	m_entitesToDestroyQueue.Clear();
}
