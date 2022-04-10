#pragma once
#include "engine/util/state/StateMachine.h"
#include <functional>
#include "game/scripts/util/CommandQueue.h"

class CutsceneState : public StateMachine<>::IState
{
public:
	using CommandFinishedFn = std::function<void()>;
	using CutsceneStateMachine = StateMachine<>;
	using CutsceneCommandQueue = CommandQueue<CutsceneStateMachine*, CommandFinishedFn>;

private:
	CutsceneCommandQueue m_commandQueue;
	std::function<void()> m_onFinished = nullptr;
	CutsceneStateMachine m_stateMachine;
	std::function<void()> m_currentCommandFinished = nullptr;

protected:
	void Enter() override;
	void Update() override;

public:
	CutsceneState(
		const CutsceneCommandQueue& commands
		, std::function<void()> onFinishedFn = nullptr
	);

	void AdvanceCommandQueue();
};
