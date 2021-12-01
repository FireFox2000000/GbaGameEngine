#pragma once

#include "engine/base/ecs/EntityComponentManager.h"

// Container for game critical systems.
class Engine
{
	// The main entity manager that holds all game scene entities and their components
	ECS::EntityComponentManager m_entityComponentManager;

	// The entity manager that just holds engine components, no sense in mixing this with scene entities
	ECS::EntityComponentManager m_engineComponentsRegistry;

	// The entity that gets assigned engine-specific components
	ECS::Entity m_engineComponentsContainer;

public:
	Engine();
	~Engine();

	ECS::EntityComponentManager* GetEntityRegistry() { return &m_entityComponentManager; }

	template<typename Component>
	inline void AddComponent()
	{
		m_engineComponentsRegistry.AddComponent<Component>(m_engineComponentsContainer);
	}

	template<typename Component, typename... Args>
	inline void AddComponent(Args&& ... args)
	{
		m_engineComponentsRegistry.AddComponent<Component>(m_engineComponentsContainer, std::forward<Args>(args)...);
	}

	template<typename Component>
	inline const Component* GetComponent() const
	{
		return m_engineComponentsRegistry.GetComponent<Component>(m_engineComponentsContainer);
	}

	template<typename Component>
	inline Component* GetComponent()
	{
		return m_engineComponentsRegistry.EditComponent<Component>(m_engineComponentsContainer);
	}
};
