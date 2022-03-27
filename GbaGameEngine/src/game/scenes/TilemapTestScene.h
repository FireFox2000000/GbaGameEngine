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

class TilemapTestScene : public Scene
{
	UiRenderer m_uiRenderer;
	CommandQueue m_uiRenderCommandQueue;

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

	std::shared_ptr<IPostProcessingGfxTask> m_kickedFadeInTask;
	std::shared_ptr<IPostProcessingGfxTask> m_kickedFadeOutTask;

	GBA::DMG::Midi::Player m_fallOfFallMidi;

public:
	TilemapTestScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Exit(Engine* engine) override;
	void Update(Engine* engine) override;
	void Render(Engine* engine) override;
};
