#include "BgFadeInState.h"
#include "engine/engine/engine.h"
#include "engine/graphics/Graphics.h"

BgFadeInState::BgFadeInState(std::function<void()> onFinishedFn)
	: m_onFinished(onFinishedFn)
{

}

void BgFadeInState::Enter()
{
}

void BgFadeInState::Update()
{
	if (!m_kickedFadeInTask)
	{
		Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();
		std::shared_ptr<GfxScreenFadeIn> fadeTask = std::make_shared<GfxScreenFadeIn>(Colour::Black, 0.25f);
		if (gfx->KickPostProcessingGfxTask(fadeTask))
		{
			m_kickedFadeInTask = fadeTask;
		}
	}

	if (m_kickedFadeInTask && m_kickedFadeInTask->IsComplete())
	{
		m_onFinished();
	}
}
