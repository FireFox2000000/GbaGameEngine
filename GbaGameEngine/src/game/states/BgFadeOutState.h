#pragma once
#include "engine/util/state/StateMachine.h"
#include <functional>
#include "game/gfx/GfxScreenFadeOut.h"

class BgFadeOutState : public StateMachine<>::IState
{
	std::shared_ptr<IPostProcessingGfxTask> m_kickedFadeOutTask;
	std::function<void()> m_onFinished = nullptr;

protected:
	void Enter() override;
	void Update() override;

public:
	BgFadeOutState(std::function<void()> onFinishedFn = nullptr);
};
