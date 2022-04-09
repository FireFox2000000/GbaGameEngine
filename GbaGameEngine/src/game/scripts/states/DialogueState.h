#pragma once
#include "engine/util/state/StateMachine.h"
#include <string>
#include <functional>

class Engine;
class UiRenderer;
class CommandQueue;

class DialogueState : public StateMachine<>::IState
{
	UiRenderer* m_uiRenderer;
	CommandQueue* m_uiRenderCommandQueue;

	std::string m_script;
	int m_totalRows = 2;

	std::function<void()> m_onFinished = nullptr;

	int m_currentTextIndex = 0;

	bool AdvanceText();

protected:
	void Enter() override;
	void Update() override;
	void Exit() override;
public:
	DialogueState(
		UiRenderer* uiRenderer
		, CommandQueue* uiRenderCommandQueue
		, const char* script
		, int totalRows
		, std::function<void()> onFinishedFn = nullptr
	);
};
