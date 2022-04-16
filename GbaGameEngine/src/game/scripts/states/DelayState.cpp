#include "DelayState.h"
#include "engine/time/Time.h"
#include "engine/engine/engine.h"

void DelayState::Enter()
{
	DEBUG_LOGFORMAT("Entered delay state for %f seconds", m_delayTime.ToFloat());
}

void DelayState::Update()
{
	const Time* time = Engine::GetInstance().GetComponent<Time>();
	tFixedPoint24 dt = time->GetDt();
	m_delayTime -= dt;
	if (m_delayTime <= 0)
	{
		m_onFinished();
	}
}

DelayState::DelayState(tFixedPoint24 delayTime, std::function<void()> onFinishedFn)
	: m_delayTime(delayTime)
	, m_onFinished(onFinishedFn)
{
}
