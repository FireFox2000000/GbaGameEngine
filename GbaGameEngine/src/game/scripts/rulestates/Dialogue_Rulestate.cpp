#include "Dialogue_Rulestate.h"
#include "engine/engine/engine.h"
#include "game/scripts/prefabs/ui/TextPrefabFunctions.h"
#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/gameobject/ui/Text.h"
#include "engine/screen/Screen.h"

#include "game/config/InputActions.h"
#include "game/scripts/componentsystems/movement/RpgMovement.h"

Dialogue_Rulestate::Dialogue_Rulestate(const std::string& script, SharedPtr<GameRulestate> finishedState, std::function<void()> onFinishedFn)
	: m_finishedState(finishedState)
	, m_onFinished(onFinishedFn)
{
	const int CHARACTERS_PER_ROW = 25;
	const int ROWS_PER_BOX = 2;

	int charIndex = 0;
	int rowIndex = 0;
	int lastSpace = -1;
	for (u32 i = 0; i < script.length(); ++i)
	{
		int stringLen = i - charIndex;
		char currentChar = script[i];

		if (currentChar == ' ')
		{
			lastSpace = i;
		}

		bool newRow = currentChar == c_dialogueBoxStepFlag;
		bool newLine = currentChar == '\n';
		bool atEnd = i == script.length() - 1;
		bool addString = stringLen > CHARACTERS_PER_ROW || newLine || newRow || atEnd;
		if (addString)
		{
			int endSubStr = !atEnd && lastSpace >= 0 ? lastSpace : i;

			m_script += script.substr(charIndex, endSubStr - charIndex);

			if (!newLine && !newRow)
				m_script += "\n";

			charIndex = endSubStr;
			i = endSubStr;
			if (script[i] == ' ')
				++i;

			lastSpace = -1;

			if (newRow)
			{
				rowIndex = 0;
			}
			else
			{
				++rowIndex;
				if (rowIndex >= ROWS_PER_BOX)
				{
					m_script += c_dialogueBoxStepFlag;
					rowIndex = 0;
				}
			}
		}
	}
}

bool Dialogue_Rulestate::AdvanceText()
{
	// Find the next set of text to display in a single box
	int dialogueIndexEnd = m_currentTextIndex;
	for (; dialogueIndexEnd < (int)m_script.length(); ++dialogueIndexEnd)
	{
		if (m_script[dialogueIndexEnd] == c_dialogueBoxStepFlag)
		{
			break;
		}
	}

	if (m_currentTextIndex == dialogueIndexEnd)
	{
		// Finished
		return true;
	}

	auto& text = *m_dialogueObject->EditComponent<Component::UI::Text>();
	text.m_str = m_script.substr(m_currentTextIndex, dialogueIndexEnd - m_currentTextIndex);

	m_currentTextIndex = dialogueIndexEnd + 1;

	return false;
}

void Dialogue_Rulestate::Enter(GameRulestateParams & params)
{
	DEBUG_LOG("Entered rulestate [Dialogue_Rulestate]");

	System::RpgMovement::StopAllMovement(params.engine);

	m_dialogueObject = std::make_unique<GameObject>(params.engine);

	TextPrefabFunctions::MakeBasicTextObj(params.engine, *m_dialogueObject.get(), FontID::DebugFont_8x8);

	Vector2<int> resolution = Screen::GetResolution();

	auto& screenTransform = *m_dialogueObject->EditComponent<Component::UI::ScreenTransform>();
	screenTransform.position.x = 4;
	screenTransform.position.y = resolution.y - 32;

	if (AdvanceText())
		params.stateMachine->ChangeState(m_finishedState, params);
}

void Dialogue_Rulestate::Update(GameRulestateParams & params)
{
	using namespace GBA;

	if (Input::GetKeyDown(InputActions::AdvanceDialogue))
	{
		if (AdvanceText())
			params.stateMachine->ChangeState(m_finishedState, params);
	}
}

void Dialogue_Rulestate::Exit(GameRulestateParams & params)
{
	if (m_onFinished)
		m_onFinished();
}
