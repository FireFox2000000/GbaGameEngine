#include "DialogueRulestate.h"
#include "game/input/Input.h"
#include "engine/engine/engine.h"
#include "engine/render/UiRenderer.h"
#include "game/scripts/util/CommandQueue.h"

const int CharactersPerRow = 20;
const char DialogueBoxStepFlag = '`';
const Vector2<int> DrawPos = Vector2<int>(1, 10);

DialogueRulestate::DialogueRulestate(
	UiRenderer* uiRenderer
	, CommandQueue* uiRenderCommandQueue
	, const char* script
	, int totalRows
	, std::function<void()> onFinishedFn
)
	: m_uiRenderer(uiRenderer)
	, m_uiRenderCommandQueue(uiRenderCommandQueue)
	, m_totalRows(totalRows)
	, m_onFinished(onFinishedFn)
{
	// Pre-process the string to find the correct places to word-wrap at. 
	int lineStartIndex = 0;
	int rowIndex = 0;
	int lastSpace = -1;

	int scriptCharCount = strlen(script);

	for (int i = 0; i < scriptCharCount; ++i)
	{
		int stringLen = i - lineStartIndex;
		char currentChar = script[i];

		if (currentChar == ' ')
		{
			lastSpace = i;
		}

		bool newRow = currentChar == DialogueBoxStepFlag;
		bool newLine = currentChar == '\n';
		bool atEnd = i == scriptCharCount - 1;
		bool addString = stringLen > CharactersPerRow || newLine || newRow || atEnd;
		if (addString)
		{
			int endSubStr = !atEnd && lastSpace >= 0 ? lastSpace : i;
			if (atEnd)
			{
				endSubStr += 1;
			}

			for (int subStrIndex = lineStartIndex; subStrIndex < endSubStr; ++subStrIndex)
			{
				m_script += script[subStrIndex];
			}

			if (!newLine && !newRow)
			{
				m_script += "\n";
			}

			lineStartIndex = endSubStr;
			i = endSubStr;

			// If the new row starts with a space, trim it
			while (i < scriptCharCount && script[i] == ' ')
			{
				++i;
				lineStartIndex = i;
			}

			lastSpace = -1;

			if (newRow)
			{
				rowIndex = 0;
			}
			else
			{
				++rowIndex;
				if (rowIndex >= m_totalRows)
				{
					m_script += DialogueBoxStepFlag;
					rowIndex = 0;
				}
			}
		}
	}
}

bool DialogueRulestate::AdvanceText()
{
	// Find the next set of text to display in a single box
	int dialogueIndexStart = m_currentTextIndex;
	int dialogueIndexEnd = m_currentTextIndex;
	for (; dialogueIndexEnd < (int)m_script.length(); ++dialogueIndexEnd)
	{
		if (m_script[dialogueIndexEnd] == DialogueBoxStepFlag)
		{
			break;
		}
	}

	if (m_currentTextIndex == dialogueIndexEnd)
	{
		// Finished
		return true;
	}

	m_uiRenderCommandQueue->Enque([this, dialogueIndexStart, dialogueIndexEnd] {
		// TODO, need to account for leaking characters, need to write over with whitespace
		m_uiRenderer->RenderText(m_script.c_str(), dialogueIndexStart, dialogueIndexEnd - dialogueIndexStart, DrawPos);
	});

	m_currentTextIndex = dialogueIndexEnd + 1;

	return false;
}

void DialogueRulestate::Enter()
{
	if (AdvanceText())
	{
		m_onFinished();
	}
}

void DialogueRulestate::Update()
{
	Input::InputManager* inputManager = Engine::GetInstance().GetComponent<Input::InputManager>();
	const auto& devices = inputManager->GetDevices();

	if (Input::GetInputDown(AdvanceDialogue, devices))
	{
		if (AdvanceText())
		{
			m_onFinished();
		}
	}
	else if (Input::GetInputDown(SkipDialogue, devices))
	{
		m_onFinished();
	}

}

void DialogueRulestate::Exit()
{
	m_uiRenderCommandQueue->Enque([this] {
		m_uiRenderer->ClearRegion(DrawPos.x, DrawPos.y, CharactersPerRow + 1, m_totalRows);// +2);	// TODO once we draw borders
	});
}
