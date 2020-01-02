#pragma once

#include "engine/scene/Scene.h"

class TilemapTestScene : public Scene
{
public:
	TilemapTestScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
