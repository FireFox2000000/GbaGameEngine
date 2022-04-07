#pragma once
#include "engine/util/state/StateMachine.h"
#include <string>
#include <functional>

class Engine;
class UiRenderer;
class CommandQueue;

class DialogueRulestate : public StateMachine<>::IState
{
	Engine* m_engine;
	UiRenderer* m_uiRenderer;
	CommandQueue* m_uiRenderCommandQueue;

	std::string m_script;
	int m_totalRows = 2;

	std::function<void()> m_onFinished = NULL;

	int m_currentTextIndex = 0;

	bool AdvanceText();

protected:
	void Enter() override;
	void Update() override;
	void Exit() override;
public:
	DialogueRulestate(
		Engine* engine
		, UiRenderer* uiRenderer
		, CommandQueue* uiRenderCommandQueue
		, const char* script
		, int totalRows
		, std::function<void()> onFinishedFn = nullptr
	);
};
