#pragma once

#include "engine/scene/Scene.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/graphics/ui/UiRenderer.h"

#include "game/gfx/GfxScreenFadeIn.h"
#include "game/gfx/GfxScreenFadeOut.h"
#include "game/util/CommandQueue.h"

#include "engine/util/state/StateMachine.h"

class CutsceneTestScene : public Scene
{
	UiRenderer m_uiRenderer;
	CommandQueue<> m_uiRenderCommandQueue;

	StateMachine<> m_stateMachine;

	GameObject m_cutsceneBg;
	GBA::Gfx::TilemapSet* m_cutsceneImg1 = nullptr;
	GBA::Gfx::TilemapSet* m_cutsceneImg2 = nullptr;

	void ChangeBg(GBA::Gfx::Tilemap* newTilemap);

public:
	CutsceneTestScene();

	void Enter() override;
	void Exit() override;
	void Update() override;
	void Render() override;
};
