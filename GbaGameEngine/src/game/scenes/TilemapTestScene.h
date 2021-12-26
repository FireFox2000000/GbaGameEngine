#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/render/UiRenderer.h"

class TilemapTestScene : public Scene
{
	UiRenderer m_uiRenderer;

	enum SpriteAnimationID
	{
		AnimationCount
	};

	enum TilemapSetID {
		Eosd,

		TilemapSetCount
	};

	FixedAssetManager<
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
