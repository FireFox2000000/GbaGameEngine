#pragma once
#include "engine/util/state/StateMachine.h"
#include <string>
#include <functional>

class Engine;
class UiRenderer;

template<typename ...>
class CommandQueue;

class DialogueState : public StateMachine<>::IState
{
public:
	struct BorderSpriteSlice
	{
		int TopLeft, TopRight, BottomLeft, BottomRight, Left, Right, Top, Bottom = 0;
	};

private:
	UiRenderer* m_uiRenderer;
	CommandQueue<>* m_uiRenderCommandQueue;

	std::string m_script;
	int m_totalRows = 2;
	BorderSpriteSlice m_borderSprites;

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
		, CommandQueue<>* uiRenderCommandQueue
		, const char* script
		, int totalRows
		, const BorderSpriteSlice& borderDef
		, std::function<void()> onFinishedFn = nullptr
	);
};
