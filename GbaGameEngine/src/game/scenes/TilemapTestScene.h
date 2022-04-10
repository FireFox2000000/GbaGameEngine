#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/render/UiRenderer.h"

#include "game/scripts/gfx/GfxScreenFadeIn.h"
#include "game/scripts/gfx/GfxScreenFadeOut.h"
#include "game/scripts/util/CommandQueue.h"

#include "engine/gba/audio/GBADmgMidiPlayer.h"
#include "engine/util/state/StateMachine.h"

class TilemapTestScene : public Scene
{
	UiRenderer m_uiRenderer;
	CommandQueue<> m_uiRenderCommandQueue;

	StateMachine<> m_stateMachine;

	enum SpriteAnimationID
	{
		AnimationCount
	};

	enum TilemapSetID {
		NightSky,
		NightSkyInverted,
		Eosd,

		TilemapSetCount
	};

	FixedAssetManager<
		SpriteAnimationID,
		TilemapSetID>
		m_assetManager;

	FixedList<GameObject, 2> m_gameObjects;
	GameObject* m_background = nullptr;

	std::shared_ptr<IPostProcessingGfxTask> m_kickedFadeInTask;
	std::shared_ptr<IPostProcessingGfxTask> m_kickedFadeOutTask;

	std::unique_ptr<GBA::DMG::Midi::Player> m_fallOfFallMidi;

public:
	TilemapTestScene();

	void Enter() override;
	void Exit() override;
	void Update() override;
	void Render() override;
};
