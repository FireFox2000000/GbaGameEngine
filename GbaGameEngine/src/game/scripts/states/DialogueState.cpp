#include "DialogueState.h"
#include "game/input/Input.h"
#include "engine/engine/engine.h"
#include "engine/render/UiRenderer.h"
#include "game/scripts/util/CommandQueue.h"

const int CharactersPerRow = 26;
const char DialogueBoxStepFlag = '`';
const Vector2i DrawPos = Vector2i(1, 15);
const Vector2i TextDrawPos = DrawPos + Vector2i(1, 1);

int FindRowEnd(const char* str, int startIndex)
{
	int lastSpace = -1;
	int strCount = strlen(str);

	for (int i = startIndex; i < strCount; ++i)
	{
		int currentCharactersOnRow = i - startIndex;
		char currentChar = str[i];

		if (currentChar == ' ')
		{
			// Save word-wrapping spot
			lastSpace = i;
		}

		if (currentCharactersOnRow >= CharactersPerRow)
		{
			return lastSpace;
		}

		if (currentChar == '\n' || currentChar == DialogueBoxStepFlag)
		{
			return i;
		}
	}

	return strCount;
}

// TODO, need to account for the size of the font character. Currently just assuming 8x8.

DialogueState::DialogueState(
	UiRenderer* uiRenderer
	, CommandQueue<>* uiRenderCommandQueue
	, const char* script
	, int totalRows
	, const BorderSpriteSlice& borderDef
	, std::function<void()> onFinishedFn
)
	: m_uiRenderer(uiRenderer)
	, m_uiRenderCommandQueue(uiRenderCommandQueue)
	, m_totalRows(totalRows)
	, m_borderSprites(borderDef)
	, m_onFinished(onFinishedFn)
{
	m_script.reserve(256);

	// Pre-process the string to find the correct places to word-wrap at.
	// Once word wrapping is found, add ' ' to fill out the rest of the row. 
	int scriptCharCount = strlen(script);
	int currentRow = 0;
	int currentCharIndex = 0;

	while (currentCharIndex < scriptCharCount)
	{
		int rowEnd = FindRowEnd(script, currentCharIndex);

		for (int subStrIndex = currentCharIndex; subStrIndex < rowEnd; ++subStrIndex)
		{
			m_script += script[subStrIndex];
		}

		// Fill out the rest with spaces
		int emptySpaceCharacters = CharactersPerRow - (rowEnd - currentCharIndex);
		while (--emptySpaceCharacters >= 0)
		{
			m_script += ' ';
		}
		m_script += '\n';
		++currentRow;

		if (script[rowEnd] == DialogueBoxStepFlag || rowEnd >= scriptCharCount)
		{
			// Populate empty rows with space characters to clear out old text
			while (currentRow++ < m_totalRows)
			{
				for (int spaceCount = 0; spaceCount < CharactersPerRow; ++spaceCount)
				{
					m_script += ' ';
				}
				m_script += '\n';
			}
		}

		if (currentRow >= m_totalRows)
		{
			currentRow = 0;
			m_script += DialogueBoxStepFlag;
		}

		currentCharIndex = rowEnd + 1;
	}
}

bool DialogueState::AdvanceText()
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
		m_uiRenderer->RenderText(m_script.c_str(), dialogueIndexStart, dialogueIndexEnd - dialogueIndexStart, TextDrawPos);
	});

	m_currentTextIndex = dialogueIndexEnd + 1;

	return false;
}

void DialogueState::Enter()
{
	// Draw border
	m_uiRenderCommandQueue->Enque([this] {
		m_uiRenderer->DrawUiElement(Vector2i(DrawPos.x, DrawPos.y), m_borderSprites.TopLeft);
		m_uiRenderer->DrawUiElement(Vector2i(DrawPos.x, DrawPos.y + 3), m_borderSprites.BottomLeft);
		m_uiRenderer->DrawUiElement(Vector2i(DrawPos.x + CharactersPerRow + 1, DrawPos.y), m_borderSprites.TopRight);
		m_uiRenderer->DrawUiElement(Vector2i(DrawPos.x + CharactersPerRow + 1, DrawPos.y + 3), m_borderSprites.BottomRight);

		for (int i = 0; i < m_totalRows; ++i)
		{
			m_uiRenderer->DrawUiElement(Vector2i(DrawPos.x, DrawPos.y + i + 1), m_borderSprites.Left);
			m_uiRenderer->DrawUiElement(Vector2i(DrawPos.x + CharactersPerRow + 1, DrawPos.y + i + 1), m_borderSprites.Right);
		}
		
		for (int i = 0; i < CharactersPerRow; ++i)
		{
			m_uiRenderer->DrawUiElement(Vector2i(DrawPos.x + i + 1, DrawPos.y), m_borderSprites.Top);
			m_uiRenderer->DrawUiElement(Vector2i(DrawPos.x + i + 1, DrawPos.y + 3), m_borderSprites.Bottom);
		}
	});

	if (AdvanceText())
	{
		m_onFinished();
	}
}

void DialogueState::Update()
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

void DialogueState::Exit()
{
	m_uiRenderCommandQueue->Enque([this] {
		m_uiRenderer->ClearRegion(DrawPos.x, DrawPos.y, CharactersPerRow + 2, m_totalRows + 2);	// TODO once we draw borders
	});
}
