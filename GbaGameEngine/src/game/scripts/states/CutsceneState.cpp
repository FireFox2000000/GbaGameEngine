#include "CutsceneState.h"

void CutsceneState::Enter()
{
	AdvanceCommandQueue();
}

void CutsceneState::Update()
{
	m_stateMachine.Update();
}

CutsceneState::CutsceneState(const CutsceneCommandQueue& commands, std::function<void()> onFinishedFn)
	: m_commandQueue(commands)
	, m_onFinished(onFinishedFn)
{
	m_currentCommandFinished = [this]() { AdvanceCommandQueue(); };
}

void CutsceneState::AdvanceCommandQueue()
{
	DEBUG_LOG("[CutsceneState::AdvanceCommandQueue]");

	if (!m_commandQueue.IsEmpty())
	{
		m_commandQueue.ExecuteNext(&m_stateMachine, m_currentCommandFinished);
	}
	else
	{
		m_stateMachine.ChangeState(nullptr);
		m_onFinished();
	}
}
