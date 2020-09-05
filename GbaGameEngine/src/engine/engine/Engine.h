#pragma once

#include "engine/base/ecs/EntityComponentManager.h"

// Container for game critical systems.
class Engine
{
	ECS::EntityComponentManager m_entityComponentManager;
	ECS::Entity m_engineComponentsContainer;

public:
	Engine();
	~Engine();

	ECS::EntityComponentManager* GetEntityRegistry() { return &m_entityComponentManager; }

	template<typename Component>
	inline void AddComponent()
	{
		m_entityComponentManager.AddComponent<Component>(m_engineComponentsContainer);
	}

	template<typename Component, typename... Args>
	inline void AddComponent(Args&& ... args)
	{
		m_entityComponentManager.AddComponent<Component>(m_engineComponentsContainer, std::forward<Args>(args)...);
	}

	template<typename Component>
	inline const Component* GetComponent() const
	{
		return m_entityComponentManager.GetComponent<Component>(m_engineComponentsContainer);
	}

	template<typename Component>
	inline Component* EditComponent()
	{
		return m_entityComponentManager.EditComponent<Component>(m_engineComponentsContainer);
	}
};
