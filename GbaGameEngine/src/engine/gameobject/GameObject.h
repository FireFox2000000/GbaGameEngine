#pragma once

#include "engine/math/Vector2.h"
#include "engine/base/core/stl/List.h"
#include "engine/component/MonoBehaviour.h"

// TODO: replace with transformation matrix
// Component system

class Engine;

class GameObject
{
	Vector2f m_localPosition;	
	List<GameObjectComponent*> m_components;
	List<MonoBehaviour*> m_monoBehaviourComponents;

public:
	GameObject();
	~GameObject();

	inline Vector2f GetPosition2() { return m_localPosition; }
	inline void SetPosition(const Vector2f& position) { m_localPosition = position; }

	void Update(Engine* engine);

	template <class T>
	T* AddComponent()
	{
		T* newComponent = new T(this);
		m_components.Add(newComponent);

		if (MonoBehaviour* component = dynamic_cast<MonoBehaviour*>(newComponent))
		{
			m_monoBehaviourComponents.Add(component);
		}

		return newComponent;
	}

	template <class T>
	T* GetComponent()
	{
		for (List<GameObjectComponent*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			if (T* component = dynamic_cast<T*>(*it))
				return component;
		}
		return NULL;
	}
};
