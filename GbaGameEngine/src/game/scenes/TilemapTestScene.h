#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"

class Tilemap;
class TilemapTestScene : public Scene
{
	FixedList<GameObject, 2> m_gameObjects;
	List<Tilemap*> m_loadedTilemaps;

	Vector2<tFixedPoint8> bgPosition;
	int row = 0;
	int column = 0;
	int tilemapDataIndex = 0;

public:
	TilemapTestScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Exit(Engine* engine) override;
	void Update(Engine* engine) override;
	void Render(Engine* engine) override;
};
