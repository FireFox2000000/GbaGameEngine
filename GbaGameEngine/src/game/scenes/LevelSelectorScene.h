#pragma once
#include "engine/scene/Scene.h"
#include "engine/render/UiRenderer.h"
#include "game/scripts/util/CommandQueue.h"

class LevelSelectorScene : public Scene
{
	UiRenderer m_uiRenderer;
	CommandQueue m_uiRenderCommandQueue;

	int m_currentSelectedIndex = 0;

	void AdjustSelectedIndex(int direction);
	void ClearNotch();
	void DrawNotch();

public:
	LevelSelectorScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
	void Render(Engine* engine) override;
};
