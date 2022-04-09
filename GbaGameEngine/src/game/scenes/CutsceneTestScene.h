#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/render/UiRenderer.h"

#include "game/scripts/gfx/GfxScreenFadeIn.h"
#include "game/scripts/gfx/GfxScreenFadeOut.h"
#include "game/scripts/util/CommandQueue.h"

#include "engine/util/state/StateMachine.h"

class CutsceneTestScene : public Scene
{
	UiRenderer m_uiRenderer;
	CommandQueue m_uiRenderCommandQueue;

	StateMachine<> m_stateMachine;

	enum SpriteAnimationID
	{
		AnimationCount
	};

	enum TilemapSetID {
		CutsceneImg1,
		CutsceneImg2,

		TilemapSetCount
	};

	FixedAssetManager<
		SpriteAnimationID,
		TilemapSetID>
		m_assetManager;

	GameObject m_cutsceneBg;

	void ChangeBg(Tilemap* newTilemap);

public:
	CutsceneTestScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Exit(Engine* engine) override;
	void Update(Engine* engine) override;
	void Render(Engine* engine) override;
};
