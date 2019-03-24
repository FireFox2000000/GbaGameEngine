#pragma once
#include "engine/component/MonoBehaviour.h"

class MovementTest : public MonoBehaviour
{
public:
	MovementTest(GameObject* gameObject);
	~MovementTest() {};

	void Update(Engine* engine);
};
