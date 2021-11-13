#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/asset/FixedAssetManager.h"

class TilemapTestScene : public Scene
{
	enum SpriteAtlusID {
		SpriteCount
	};


	enum SpriteAnimationID
	{
		AnimationCount
	};

	enum TilemapSetID {
		Eosd,

		TilemapSetCount
	};

	FixedAssetManager<
		SpriteAtlusID,
		SpriteAnimationID,
		TilemapSetID>
		m_assetManager;

	FixedList<GameObject, 2> m_gameObjects;

public:
	TilemapTestScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Exit(Engine* engine) override;
	void Update(Engine* engine) override;
	void Render(Engine* engine) override;
};
