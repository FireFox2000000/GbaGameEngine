#ifndef PRAGMA_ONCE_ENGINE_GAMEOBJECT_GAMEOBJECT_H
#define PRAGMA_ONCE_ENGINE_GAMEOBJECT_GAMEOBJECT_H

#include "engine/math/Vector2.h"
#include "engine/base/core/stl/List.h"
#include "engine/component/MonoBehaviour.h"

// TODO: replace with transformation matrix
// Component system

class GameObject
{
	Vector2 m_localPosition;	
	List<GameObjectComponent*> m_components;
	List<MonoBehaviour*> m_monoBehaviourComponents;

public:
	GameObject();
	~GameObject();

	inline Vector2 GetPosition2() { return m_localPosition; }
	inline void SetPosition(const Vector2& position) { m_localPosition = position; }

	void Update();

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

#endif
