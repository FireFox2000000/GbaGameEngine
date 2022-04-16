#pragma once
#include "engine/util/state/StateMachine.h"
#include "engine/base/core/stl/FixedPoint.h"
#include <functional>

class DelayState : public StateMachine<>::IState
{
	tFixedPoint24 m_delayTime = 0;
	std::function<void()> m_onFinished = nullptr;

protected:
	void Enter() override;
	void Update() override;

public:
	DelayState(tFixedPoint24 delayTime
	, std::function<void()> onFinishedFn = nullptr);
};
