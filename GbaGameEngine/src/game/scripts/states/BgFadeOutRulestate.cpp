#include "BgFadeOutRulestate.h"
#include "engine/engine/engine.h"
#include "engine/graphics/Graphics.h"

BgFadeOutRulestate::BgFadeOutRulestate(std::function<void()> onFinishedFn)
	: m_onFinished(onFinishedFn)
{
}

void BgFadeOutRulestate::Enter()
{
	DEBUG_LOG("Entered rulestate [BgFadeOutRulestate]");

	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
	std::shared_ptr<GfxScreenFadeOut> fadeTask = std::make_shared<GfxScreenFadeOut>(Colour::Black, 0.25f);
	if (gfx->KickPostProcessingGfxTask(fadeTask))
	{
		m_kickedFadeOutTask = fadeTask;
	}
}

void BgFadeOutRulestate::Update()
{
	if (m_kickedFadeOutTask->IsComplete())
	{
		m_onFinished();
	}
}
