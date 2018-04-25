#ifndef PRAGMA_ONCE_ENGINE_COMPONENT_GAMEOBJECTCOMPONENT_H
#define PRAGMA_ONCE_ENGINE_COMPONENT_GAMEOBJECTCOMPONENT_H

class GameObject;
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

#endif
