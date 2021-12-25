#pragma once
#include "engine/scene/Scene.h"
#include "engine/render/UiRenderer.h"

class UiAtlusTestScene : public Scene
{
	UiRenderer m_uiRenderer;

public:
	UiAtlusTestScene(Engine* engine);

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
};
