#ifndef PRAGMA_ONCE_ENGINE_COMPONENT_MONOBEHAVIOUR_H
#define PRAGMA_ONCE_ENGINE_COMPONENT_MONOBEHAVIOUR_H

#include "GameObjectComponent.h"

class Engine;
class MonoBehaviour : public GameObjectComponent
{
public:
	MonoBehaviour(GameObject* gameObject);
	virtual ~MonoBehaviour();

	virtual void Update(Engine* engine) {};
};

#endif
