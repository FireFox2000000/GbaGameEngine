#pragma once
#include "engine/scene/Scene.h"
#include "engine/graphics/ui/UiRenderer.h"
#include "game/scripts/util/CommandQueue.h"

class LevelSelectorScene : public Scene
{
	UiRenderer m_uiRenderer;
	CommandQueue<> m_uiRenderCommandQueue;

	int m_currentSelectedIndex = 0;

	void AdjustSelectedIndex(int direction);
	void ClearNotch();
	void DrawNotch();

public:
	LevelSelectorScene();

	void Enter() override;
	void Update() override;
	void Render() override;
};
