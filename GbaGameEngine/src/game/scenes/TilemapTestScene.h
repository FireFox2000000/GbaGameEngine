#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/graphics/ui/UiRenderer.h"

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

	FixedList<GameObject, 2> m_gameObjects;
	GameObject* m_background = nullptr;
	GBA::Gfx::TilemapSet* m_nightSkyTilemapSet = nullptr;
	GBA::Gfx::TilemapSet* m_eosdTilemapSet = nullptr;

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
