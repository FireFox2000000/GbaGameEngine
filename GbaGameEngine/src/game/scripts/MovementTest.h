#ifndef PRAGMA_ONCE_GAME_SCRIPTS_MOVEMENTTEST_H
#define PRAGMA_ONCE_GAME_SCRIPTS_MOVEMENTTEST_H

#include "engine/component/MonoBehaviour.h"

class MovementTest : public MonoBehaviour
{
public:
	MovementTest(GameObject* gameObject);
	~MovementTest() {};

	void Update(Engine* engine);
};

#endif
