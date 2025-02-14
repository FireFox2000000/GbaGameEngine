#include "BgFadeOutState.h"
#include "engine/engine/engine.h"
#include "engine/graphics/Graphics.h"

BgFadeOutState::BgFadeOutState(std::function<void()> onFinishedFn)
	: m_onFinished(onFinishedFn)
{
}

void BgFadeOutState::Enter()
{
	DEBUG_LOG("Entered rulestate [BgFadeOutState]");

	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
	std::shared_ptr<GfxScreenFadeOut> fadeTask = std::make_shared<GfxScreenFadeOut>(Colour::Black, 0.25f);
	if (gfx->KickPostProcessingGfxTask(fadeTask))
	{
		m_kickedFadeOutTask = fadeTask;
	}
}

void BgFadeOutState::Update()
{
	if (m_kickedFadeOutTask->IsComplete())
	{
		m_onFinished();
	}
}
