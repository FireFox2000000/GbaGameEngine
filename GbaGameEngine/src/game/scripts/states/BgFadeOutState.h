#pragma once
#include "engine/util/state/StateMachine.h"
#include <functional>
#include "game/scripts/gfx/GfxScreenFadeOut.h"

class Engine;

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
