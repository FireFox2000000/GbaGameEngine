#pragma once

class GameObject;
class Engine;
class GameObjectComponent
{
	GameObject* m_gameObject;

protected:
	friend class GameObject;
	GameObjectComponent(GameObject* gameObject);
public:
	inline GameObject * GetGameObject() { return m_gameObject; }
	virtual ~GameObjectComponent();
};
