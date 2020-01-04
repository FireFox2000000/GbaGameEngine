#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"

class TilemapTestScene : public Scene
{
	Vector2<tFixedPoint8> bgPosition;

public:
	TilemapTestScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
