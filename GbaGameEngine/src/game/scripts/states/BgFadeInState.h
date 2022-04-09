#pragma once
#include "engine/util/state/StateMachine.h"
#include <functional>
#include "game/scripts/gfx/GfxScreenFadeIn.h"

class Engine;

class BgFadeInState : public StateMachine<>::IState
{
	std::shared_ptr<IPostProcessingGfxTask> m_kickedFadeInTask;
	std::function<void()> m_onFinished = nullptr;

protected:
	void Enter() override;
	void Update() override;

public:
	BgFadeInState(
		std::function<void()> onFinishedFn = nullptr
	);
};
