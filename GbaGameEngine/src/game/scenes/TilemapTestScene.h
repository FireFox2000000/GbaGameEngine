#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/graphicalassets/Tilemap/TilemapSet.h"

class Tilemap;
class TilemapTestScene : public Scene
{
	FixedList<GameObject, 2> m_gameObjects;
	
	FixedList<TilemapSet, 1> m_tilemapSets;
	List<Tilemap*> m_loadedTilemaps;

public:
	TilemapTestScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Exit(Engine* engine) override;
	void Update(Engine* engine) override;
	void Render(Engine* engine) override;
};
