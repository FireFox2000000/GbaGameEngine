#pragma once

#include "GameObjectComponent.h"

class Engine;
class MonoBehaviour : public GameObjectComponent
{
public:
	MonoBehaviour(GameObject* gameObject);
	virtual ~MonoBehaviour();

	virtual void Update(Engine* engine) {};
};
